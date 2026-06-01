// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "D4_HD3_CustomProjectCharacter.h"


void UPlayerUI::UpdateValues()
{
	if (Player)
	{
		ExperienceBar->SetPercent(Player->Experience / Player->MaxExperienceLevel);
		CurrentExperienceText->SetText(FText::FromString(FString::FromInt(Player->Experience)));
		MaxExperienceText->SetText(FText::FromString(FString::FromInt(Player->MaxExperienceLevel)));
	}
}
