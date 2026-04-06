#include "MyGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UMyGameInstance::Init()
{
    Super::Init();
}

// ===================================================================
//  LOGIN
// ===================================================================

void UMyGameInstance::Login()
{
    IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
    if (!OSS)
    {
        UE_LOG(LogTemp, Error, TEXT("No OnlineSubsystem found"));
        return;
    }

    IOnlineIdentityPtr Identity = OSS->GetIdentityInterface();
    if (!Identity.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("No Identity interface"));
        return;
    }

    // Bind delegate
    Identity->OnLoginCompleteDelegates->AddUObject(this, &UMyGameInstance::OnLoginComplete);

    // Read from command line
    FString AuthType, AuthLogin, AuthPassword;

    FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE="), AuthType);
    FParse::Value(FCommandLine::Get(), TEXT("AUTH_LOGIN="), AuthLogin);
    FParse::Value(FCommandLine::Get(), TEXT("AUTH_PASSWORD="), AuthPassword);

    // Fallback (optional)
    if (AuthType.IsEmpty()) AuthType = TEXT("developer");

    UE_LOG(LogTemp, Warning, TEXT("AuthType: %s"), *AuthType);
    UE_LOG(LogTemp, Warning, TEXT("AuthLogin: %s"), *AuthLogin);
    UE_LOG(LogTemp, Warning, TEXT("AuthPassword: %s"), *AuthPassword);

    // Apply credentials
    FOnlineAccountCredentials Credentials;
    Credentials.Type = AuthType;
    Credentials.Id = AuthLogin;
    Credentials.Token = AuthPassword;

    UE_LOG(LogTemp, Warning, TEXT("Logging in using command line credentials..."));
    Identity->Login(0, Credentials);
}

void UMyGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
    UE_LOG(LogTemp, Warning, TEXT("Login Result: %s  %s"),
        bWasSuccessful ? TEXT("Success") : TEXT("FAILED"), *Error);

    // Unbind so we don't get called again
    if (IOnlineSubsystem* OSS = IOnlineSubsystem::Get())
    {
        if (IOnlineIdentityPtr Identity = OSS->GetIdentityInterface())
        {
            Identity->ClearOnLoginCompleteDelegates(0, this);
        }
    }

    bIsLoggedIn = bWasSuccessful;
    OnLoginCompleted.Broadcast(bWasSuccessful);

    if (bWasSuccessful)
    {
        // Auto-host for testing — remove or gate behind UI later
        //HostSession();
    }
}

// ===================================================================
//  HOST SESSION
// ===================================================================

void UMyGameInstance::HostSession()
{
    IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
    if (!OSS) return;

    IOnlineSessionPtr Sessions = OSS->GetSessionInterface();
    if (!Sessions.IsValid()) return;

    // Destroy any existing session first (safety)
    Sessions->DestroySession(NAME_GameSession);

    // Bind create-session delegate
    Sessions->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnCreateSessionComplete);

    FOnlineSessionSettings SessionSettings;
    SessionSettings.NumPublicConnections = 2;
    SessionSettings.bIsLANMatch = false;
    SessionSettings.bUsesPresence = true;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bAllowJoinViaPresence = true;
    SessionSettings.bUseLobbiesIfAvailable = true;

    UE_LOG(LogTemp, Warning, TEXT("Creating session..."));
    Sessions->CreateSession(0, NAME_GameSession, SessionSettings);
}

void UMyGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    UE_LOG(LogTemp, Warning, TEXT("Create Session '%s': %s"),
        *SessionName.ToString(), bWasSuccessful ? TEXT("Success") : TEXT("FAILED"));

    // Unbind
    if (IOnlineSubsystem* OSS = IOnlineSubsystem::Get())
    {
        if (IOnlineSessionPtr Sessions = OSS->GetSessionInterface())
        {
            Sessions->ClearOnCreateSessionCompleteDelegates(this);
        }
    }

    if (bWasSuccessful)
    {
        // Open your map as a listen server
        UGameplayStatics::OpenLevel(GetWorld(), "Gameplay", true, "listen");
    }
}

// ===================================================================
//  FIND SESSIONS
// ===================================================================

void UMyGameInstance::FindSessions()
{
    IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
    if (!OSS) return;

    IOnlineSessionPtr Sessions = OSS->GetSessionInterface();
    if (!Sessions.IsValid()) return;

    // Bind find-sessions delegate
    Sessions->OnFindSessionsCompleteDelegates.AddUObject(this, &UMyGameInstance::OnFindSessionsComplete);

    SearchSettings = MakeShareable(new FOnlineSessionSearch());
    SearchSettings->MaxSearchResults = 10;
    SearchSettings->bIsLanQuery = false;
    SearchSettings->QuerySettings.Set(FName(TEXT("PRESENCESEARCH")), true, EOnlineComparisonOp::Equals);

    UE_LOG(LogTemp, Warning, TEXT("Finding sessions..."));
    Sessions->FindSessions(0, SearchSettings.ToSharedRef());
}

void UMyGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
    UE_LOG(LogTemp, Warning, TEXT("Find Sessions: %s — Results: %d"),
        bWasSuccessful ? TEXT("Success") : TEXT("FAILED"),
        SearchSettings.IsValid() ? SearchSettings->SearchResults.Num() : 0);

    // Unbind
    if (IOnlineSubsystem* OSS = IOnlineSubsystem::Get())
    {
        if (IOnlineSessionPtr Sessions = OSS->GetSessionInterface())
        {
            Sessions->ClearOnFindSessionsCompleteDelegates(this);
        }
    }

    if (bWasSuccessful && SearchSettings.IsValid() && SearchSettings->SearchResults.Num() > 0)
    {
        // Auto-join the first result for testing
        IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
        if (!OSS) return;

        IOnlineSessionPtr Sessions = OSS->GetSessionInterface();
        if (!Sessions.IsValid()) return;

        Sessions->OnJoinSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnJoinSessionComplete);

        UE_LOG(LogTemp, Warning, TEXT("Joining first found session..."));
        Sessions->JoinSession(0, NAME_GameSession, SearchSettings->SearchResults[0]);
    }
}

// ===================================================================
//  JOIN SESSION
// ===================================================================

void UMyGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    UE_LOG(LogTemp, Warning, TEXT("Join Session '%s': %d"), *SessionName.ToString(), (int32)Result);

    IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
    if (!OSS) return;

    IOnlineSessionPtr Sessions = OSS->GetSessionInterface();
    if (!Sessions.IsValid()) return;

    // Unbind
    Sessions->ClearOnJoinSessionCompleteDelegates(this);

    if (Result != EOnJoinSessionCompleteResult::Success) return;

    // Get the connect string and travel to it
    FString URL;
    if (Sessions->GetResolvedConnectString(SessionName, URL))
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PC)
        {
            UE_LOG(LogTemp, Warning, TEXT("Traveling to: %s"), *URL);
            PC->ClientTravel(URL, TRAVEL_Absolute);
        }
    }
}