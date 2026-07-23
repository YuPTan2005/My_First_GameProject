// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWonUI.h"

void UPlayerWonUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	BackgroundButton->SetVisibility(ESlateVisibility::Visible);
	
	BackToMainMenuButton->OnClicked.AddDynamic(this, &UPlayerWonUI::BackToMainMenu);
}

void UPlayerWonUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	UIContainer->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerWonUI::BackToMainMenu()
{
	
}

void UPlayerWonUI::ShowUIWidget() const
{
	UIContainer->SetVisibility(ESlateVisibility::Visible);
}
