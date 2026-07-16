// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

void UEnemyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	if (APawn* Owner = TryGetPawnOwner())
	{
		Character = Cast<AEnemy>(Owner);
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (Character)
	{
		bIsFlying = Character->bIsFlying;
		Speed = Character->GetVelocity().Length();
		bCanAttack = Character->bCanAttack;
		bIsDead = Character->bIsDead;
	}
}
