// Fill out your copyright notice in the Description page of Project Settings.


#include "CompanionAnimInstance.h"

void UCompanionAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	if (APawn* Owner = TryGetPawnOwner())
	{
		Companion = Cast<ACompanion>(Owner);
	}
}

void UCompanionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (Companion)
	{
		Speed = Companion->GetVelocity().Length();
		bCanAttack = Companion->CanAttack();
		bIsDead = IDamageable::Execute_IsDead(Companion);
	}
}
