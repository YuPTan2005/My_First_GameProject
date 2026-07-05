// Fill out your copyright notice in the Description page of Project Settings.


#include "CompanionAnimInstance.h"

void UCompanionAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	Companion = Cast<ACompanion>(TryGetPawnOwner());
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
