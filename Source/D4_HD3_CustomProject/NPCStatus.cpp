// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCStatus.h"

void UNPCStatus::UpdateValues()
{
	if (BindingActor && BindingActor->Implements<UNPCInterface>())
	{
		HealthBar->SetPercent(INPCInterface::Execute_GetCurrentHealth(BindingActor) /
			INPCInterface::Execute_GetMaxHealth(BindingActor));
		CurrentHealthText->SetText(FText::FromString(FString::FromInt
			(INPCInterface::Execute_GetCurrentHealth(BindingActor))));
		MaxHealthText->SetText(FText::FromString(FString::FromInt
			(INPCInterface::Execute_GetMaxHealth(BindingActor))));
	}
}

void UNPCStatus::SetHealthBarSize(FVector2D NewSize)
{
	if (HealthBarSizeBox)
	{
		HealthBarSizeBox->SetWidthOverride(NewSize.X);
		HealthBarSizeBox->SetHeightOverride(NewSize.Y);
	}
}

void UNPCStatus::SetMaxHealthTextSize(float NewSize)
{
	FSlateFontInfo FontInfo = MaxHealthText->GetFont();
	FontInfo.Size = NewSize;
	
	MaxHealthText->SetFont(FontInfo);
}

void UNPCStatus::SetCurrentHealthTextSize(float NewSize)
{
	FSlateFontInfo FontInfo = CurrentHealthText->GetFont();
	FontInfo.Size = NewSize;
	
	CurrentHealthText->SetFont(FontInfo);
}
