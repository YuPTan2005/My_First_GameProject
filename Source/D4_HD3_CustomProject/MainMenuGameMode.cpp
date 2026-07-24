// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "Blueprint/UserWidget.h"

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (MainMenuClass && Controller)
	{
		if (UUserWidget* MainMenu = CreateWidget(GetGameInstance(), MainMenuClass))
		{
			MainMenu->AddToViewport();
		
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(MainMenu->TakeWidget());
			Controller->SetInputMode(InputMode);
			Controller->SetShowMouseCursor(true);
		}
	}
}
