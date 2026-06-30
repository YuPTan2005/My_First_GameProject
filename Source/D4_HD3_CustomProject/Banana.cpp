// Fill out your copyright notice in the Description page of Project Settings.


#include "Banana.h"

ABanana::ABanana()
{
	FoodName = "Banana";
	FoodDescription = "Experience amount: 50. Starvation amount: 30. Allow dashing for 10 seconds. (Press Shift to dash)";
	ExperienceAmount = 80;
	StarvationAmount = 30;
}

void ABanana::EatenBy_Implementation(ACharacter* Character)
{
	Super::EatenBy_Implementation(Character);
	
	Player = Cast<AD4_HD3_CustomProjectCharacter>(Character);
	if (Player)
	{
		Player->SetDashSpeed(DashSpeed);
		Player->SetCanDash(true);
		Player->SetDashTimer(Player->GetDashCoolDown());
		GetWorld()->GetTimerManager().SetTimer(
			DashTimer,
			this,
			&ABanana::RemoveDashAbility,
			EffectDuration,
			false
			);
	}
}

void ABanana::RemoveDashAbility()
{
	Player->SetCanDash(false);
	Player->SetDashSpeed(1.0f);
}


