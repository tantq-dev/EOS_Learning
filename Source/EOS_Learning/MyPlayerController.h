// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UMainMenuWidget;
/**
 * 
 */
UCLASS()
class EOS_LEARNING_API AMyPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    // This allows you to pick your WBP_MainMenu in the Details panel of a BP child
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UMainMenuWidget* MainMenuWidget;
	
};
