// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "D4_HD3_CustomProjectCharacter.h"


void UPlayerUI::UpdateValues() const
{
	UpdatePlayerValues();
	UpdateCompanionValues();
}

void UPlayerUI::UpdatePlayerValues() const
{
	if (Player)
	{
		ExperienceBar->SetPercent(Player->GetExperience() / Player->GetMaxExperience());
		CurrentExperienceText->SetText(FText::FromString(FString::FromInt(Player->GetExperience())));
		MaxExperienceText->SetText(FText::FromString(FString::FromInt(Player->GetMaxExperience())));
		
		HealthBar->SetPercent(Player->GetCurrentHealth() / Player->GetMaxHealth());
		CurrentHealthText->SetText(FText::FromString(FString::FromInt(Player->GetCurrentHealth())));
		MaxHealthText->SetText(FText::FromString(FString::FromInt(Player->GetMaxHealth())));
		
		PlayerLevelText->SetText(FText::FromString(FString::FromInt(Player->GetCurrentLevel())));
		
		PlayerDamageText->SetText(FText::FromString(FString::FromInt(Player->Damage)));
		
		StarvationBar->SetPercent(Player->GetStarvationValue() / Player->GetMaxStarvationValue());
	}
}

void UPlayerUI::UpdateCompanionValues() const
{
	if (Companion)
	{
		CompanionLevelText->SetText(FText::FromString(FString::FromInt(Companion->GetCurrentLevel())));
		CompanionDamageText->SetText(FText::FromString(FString::FromInt(Companion->GetDamageValue())));
	}
	else
	{
		CompanionLevelText->SetText(FText::FromString("-"));
		CompanionDamageText->SetText(FText::FromString("-"));
	}
}

void UPlayerUI::SetCompanion(ACompanion* NewCompanion)
{
	Companion = NewCompanion;
}
