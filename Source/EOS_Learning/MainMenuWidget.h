// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UWidgetSwitcher;
class UButton;
/**
 * 
 */
UCLASS()
class EOS_LEARNING_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UWidgetSwitcher* MenuSwitcher;

    // Bind these to your WBP button names
    UPROPERTY(meta = (BindWidget))
    UButton* CreateButton;

    UPROPERTY(meta = (BindWidget))
    UButton* JoinButton;

    virtual void NativeConstruct() override;

    UFUNCTION()
    void UpdateUI(bool bWasSuccessful);

    UFUNCTION()
    void OnCreateClicked();

    UFUNCTION()
    void OnJoinClicked();
	
};
