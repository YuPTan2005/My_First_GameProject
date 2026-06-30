// Fill out your copyright notice in the Description page of Project Settings.


#include "Pineapple.h"

APineapple::APineapple()
{
	FoodName = "Pineapple";
	FoodDescription = TEXT("Experience amount: 20. Starvation value will stop to drop for 20 seconds.");
	ExperienceAmount = 20;
	StarvationAmount = 0; // Will directly set the value to max
}

void APineapple::EatenBy_Implementation(ACharacter* Character)
{
	Super::EatenBy_Implementation(Character);
	
	Player = Cast<AD4_HD3_CustomProjectCharacter>(Character);
	if (Player)
	{
		Player->StarvationValue = Player->MaxStarvationValue;
		Player->SetIsStarvationDecrement(false);
		GetWorld()->GetTimerManager().SetTimer(
			StarvationTimer,
			this,
			&APineapple::ContinueStarvationDecrement,
			EffectDuration,
			false
			);
	}
}

void APineapple::ContinueStarvationDecrement()
{
	Player->bIsStarvationDecrement = true;
}


