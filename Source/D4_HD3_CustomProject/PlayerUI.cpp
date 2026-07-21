// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "D4_HD3_CustomProjectCharacter.h"


void UPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	BorderOriginalColor = Weapon1Border->GetBrushColor();
}

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
		
		HealthBar->SetPercent(Player->GetCurrentHealth_Implementation() / Player->GetMaxHealth_Implementation());
		CurrentHealthText->SetText(FText::FromString(FString::FromInt(Player->GetCurrentHealth_Implementation())));
		MaxHealthText->SetText(FText::FromString(FString::FromInt(Player->GetMaxHealth_Implementation())));
		
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

bool UPlayerUI::SetNewWeaponImage(UTexture2D* NewWeaponImage) const
{
	if (Weapon1Image->GetBrush().GetResourceObject() == nullptr)
	{
		Weapon1Image->SetBrushFromTexture(NewWeaponImage);
		return true;
	}
	if (Weapon2Image->GetBrush().GetResourceObject() == nullptr)
	{
		Weapon2Image->SetBrushFromTexture(NewWeaponImage);
		return true;
	}
	
	return false;
}

void UPlayerUI::RemoveWeaponImage(int8 WeaponIndex) const
{
	if (WeaponIndex == 0)
	{
		Weapon1Image->SetBrushFromTexture(nullptr);
	}
	else if (WeaponIndex == 1)
	{
		Weapon2Image->SetBrushFromTexture(nullptr);
	}
}

void UPlayerUI::ResetWeaponBorderColor(int8 WeaponIndex) const
{
	if (WeaponIndex == 0)
	{
		Weapon1Border->SetBrushColor(BorderOriginalColor);
	}
	else if (WeaponIndex == 1)
	{
		Weapon2Border->SetBrushColor(BorderOriginalColor);
	}
}

void UPlayerUI::SetWeaponBorderColor(const int8 WeaponIndex, FLinearColor BorderColor) const
{
	if (WeaponIndex == 0)
	{
		Weapon1Border->SetBrushColor(BorderColor);
	}
	else
	{
		Weapon2Border->SetBrushColor(BorderColor);
	}
}

void UPlayerUI::SetWeaponUIVisibility(const bool bIsVisible) const
{
	if (bIsVisible)
	{
		Weapon1Border->SetVisibility(ESlateVisibility::Visible);
		Weapon2Border->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Weapon1Border->SetVisibility(ESlateVisibility::Hidden);
		Weapon2Border->SetVisibility(ESlateVisibility::Hidden);
	}
}
