#include "MainMenuWidget.h"
#include "MyGameInstance.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CreateButton)
    {
        CreateButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCreateClicked);
    }

    if (JoinButton)
    {
        JoinButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnJoinClicked);
    }

    if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
    {
        if (GI->bIsLoggedIn)
        {
            MenuSwitcher->SetActiveWidgetIndex(1);
        }
        else
        {
            MenuSwitcher->SetActiveWidgetIndex(0);
            GI->OnLoginCompleted.AddDynamic(this, &UMainMenuWidget::UpdateUI);
        }
    }
}

void UMainMenuWidget::UpdateUI(bool bWasSuccessful)
{
    if (bWasSuccessful && MenuSwitcher)
    {
        MenuSwitcher->SetActiveWidgetIndex(1);
    }
}

void UMainMenuWidget::OnCreateClicked()
{
    if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
    {
        GI->HostSession(); // You must implement this
    }
}

void UMainMenuWidget::OnJoinClicked()
{
    if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
    {
        GI->FindSessions(); // You must implement this
    }
}