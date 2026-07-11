// Fill out your copyright notice in the Description page of Project Settings.


#include "CompanionStatusUI.h"
#include "Companion.h"

void UCompanionStatusUI::UpdateValues() const
{
	if (BindingActor)
	{
		HealthBar->SetPercent(BindingActor->GetCurrentHealth() /
			BindingActor->GetMaxHealth());
		CurrentHealthText->SetText(FText::FromString(FString::FromInt
			(BindingActor->GetCurrentHealth())));
		MaxHealthText->SetText(FText::FromString(FString::FromInt
			(BindingActor->GetMaxHealth())));
		
		StarvationBar->SetPercent(BindingActor->GetStarvationValue() /
			BindingActor->GetMaxStarvationValue());
		CurrentStarvationText->SetText(FText::FromString(FString::FromInt
			(BindingActor->GetStarvationValue())));
		MaxStarvationText->SetText(FText::FromString(FString::FromInt
			(BindingActor->GetMaxStarvationValue())));
	}
}

void UCompanionStatusUI::SetMaxHealthTextSize(const float NewSize) const
{
	FSlateFontInfo FontInfo = MaxHealthText->GetFont();
	FontInfo.Size = NewSize;
	
	MaxHealthText->SetFont(FontInfo);
}

void UCompanionStatusUI::SetCurrentHealthTextSize(const float NewSize) const
{
	FSlateFontInfo FontInfo = CurrentHealthText->GetFont();
	FontInfo.Size = NewSize;
	
	CurrentHealthText->SetFont(FontInfo);
}

void UCompanionStatusUI::SetMaxStarvationTextSize(float NewSize) const
{
	FSlateFontInfo FontInfo = MaxHealthText->GetFont();
	FontInfo.Size = NewSize;
	
	MaxStarvationText->SetFont(FontInfo);
}

void UCompanionStatusUI::SetCurrentStarvationTextSize(float NewSize) const
{
	FSlateFontInfo FontInfo = MaxHealthText->GetFont();
	FontInfo.Size = NewSize;
	
	CurrentStarvationText->SetFont(FontInfo);
}
