// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MainMenuWidget.h"
#include "MyGameInstance.h"

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalController() && MainMenuWidgetClass)
    {
        // 1. Create the Widget
        MainMenuWidget = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetClass);

        if (MainMenuWidget)
        {
            // 2. Add to Screen
            MainMenuWidget->AddToViewport();

            // 3. Setup Input Mode (Allow mouse to interact with UI)
            FInputModeGameAndUI InputMode;
            InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

            SetInputMode(InputMode);

            // 4. Show the Mouse Cursor
            bShowMouseCursor = true;

            // UI is now on screen, safe to start login
            if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
            {
                GI->Login();
            }

        }
    }
}