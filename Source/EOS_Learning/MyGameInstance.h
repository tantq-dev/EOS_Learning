// MyGameInstance.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MyGameInstance.generated.h"

class FOnlineSessionSearch;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginCompleted, bool, bSuccessful);

UCLASS()
class EOS_LEARNING_API UMyGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;

    // --- Login ---
    void Login();

    // --- Host ---
    UFUNCTION(BlueprintCallable)
    void HostSession();

    // --- Find / Join ---
    UFUNCTION(BlueprintCallable)
    void FindSessions();

    // Broadcast so MainMenuWidget can react
    UPROPERTY(BlueprintAssignable)
    FOnLoginCompleted OnLoginCompleted;

    // Checked by MainMenuWidget on construct
    UPROPERTY(BlueprintReadOnly)
    bool bIsLoggedIn = false;

private:
    // OSS callback handlers
    void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
    void OnFindSessionsComplete(bool bWasSuccessful);
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    // Stored search results
    TSharedPtr<FOnlineSessionSearch> SearchSettings;
};