// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingPotion.h"

#include "D4_HD3_CustomProjectCharacter.h"

AHealingPotion::AHealingPotion()
{
	HealingPercent = 0.4f;
	MaxHealAmount = 100.0f;
	FoodName = "Healing Potion";
	FoodDescription = FString::Printf(
		TEXT("Heal %.1f%% of your maximum health. Maximum healing amount: %.1f."), 
		HealingPercent * 100.0f,
		MaxHealAmount
	);
	ExperienceAmount = 0;
	StarvationAmount = 0;
}

void AHealingPotion::EatenBy_Implementation(AActor* Character)
{
	Super::EatenBy_Implementation(Character);
	
	if (Character->Implements<UFoodConsumer>())
	{
		const float PlayerMaxHealth = IFoodConsumer::Execute_GetMaxHealth(Character);
		float HealAmount = PlayerMaxHealth * HealingPercent;
		
		if (HealAmount > MaxHealAmount) HealAmount = MaxHealAmount;
		if (HealAmount > PlayerMaxHealth) HealAmount = PlayerMaxHealth;
		
		float PlayerHealth = IFoodConsumer::Execute_GetCurrentHealth(Character) + HealAmount;
		if (PlayerHealth > PlayerMaxHealth) PlayerHealth = PlayerMaxHealth;
		IFoodConsumer::Execute_SetCurrentHealth(Character, PlayerHealth);
	}
}
