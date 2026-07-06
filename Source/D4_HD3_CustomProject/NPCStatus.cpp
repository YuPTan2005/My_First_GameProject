// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCStatus.h"

void UNPCStatus::UpdateValues()
{
	if (Player)
	{
		HealthBar->SetPercent(Player->CurrentHealth /
			Player->MaxHealth);
		CurrentHealthText->SetText(FText::FromString(FString::FromInt
			(Player->CurrentHealth)));
		MaxHealthText->SetText(FText::FromString(FString::FromInt
			(Player->MaxHealth)));
	}
}
