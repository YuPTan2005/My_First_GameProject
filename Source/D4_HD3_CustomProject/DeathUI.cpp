// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathUI.h"

#include "D4_HD3_CustomProjectCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

void UDeathUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	BackgroundButton->SetVisibility(ESlateVisibility::Visible);
	
	ReplayButton->OnClicked.AddDynamic(this, &UDeathUI::OnReplayButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &UDeathUI::OnQuitButtonClicked);
}

void UDeathUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	UIContainer->SetVisibility(ESlateVisibility::Hidden);
}

void UDeathUI::OnReplayButtonClicked()
{
	RemoveFromParent();
	Owner->Destroy();
	
	if (LoadingPage)
	{
		if (APlayerController* Controller = GetWorld()->GetFirstPlayerController())
		{
			if (URespawnLoadingUI* LoadingPageWidget = CreateWidget<URespawnLoadingUI>(GetGameInstance(), LoadingPage))
			{
				LoadingPageWidget->AddToViewport();
			
				Controller->SetShowMouseCursor(false);
				
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(LoadingPageWidget->TakeWidget());
				Controller->SetInputMode(InputMode);
			
				LoadingPageWidget->StartLoading();
			}
		}
	}
}

void UDeathUI::OnQuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		GetWorld()->GetFirstPlayerController(),
		EQuitPreference::Quit,
		false
		);
}

void UDeathUI::ShowUIWidget() const
{
	UIContainer->SetVisibility(ESlateVisibility::Visible);
}
