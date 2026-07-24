// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathUI.h"

#include "D4_HD3_CustomProjectCharacter.h"
#include "Kismet/GameplayStatics.h"

void UDeathUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	BackgroundButton->SetVisibility(ESlateVisibility::Visible);
	
	ReplayButton->OnClicked.AddDynamic(this, &UDeathUI::OnReplayButtonClicked);
	BackToMainMenuButton->OnClicked.AddDynamic(this, &UDeathUI::OnBackToMainMenuButtonClicked);
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

void UDeathUI::OnBackToMainMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(this, FName("L_MainMenu"));
}

void UDeathUI::ShowUIWidget() const
{
	UIContainer->SetVisibility(ESlateVisibility::Visible);
}
