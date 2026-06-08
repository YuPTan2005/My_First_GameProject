// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "BrainComponent.h"
#include "D4_HD3_CustomProjectCharacter.h"
#include "EnemyAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanAttack = false;
	bIsDead = false;
	
	GetCharacterMovement()->MaxWalkSpeed = 400;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::Ragdoll()
{
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	
	Cast<AEnemyAIController>(GetController())->BrainComponent->PauseLogic("Ragdolling!");
}

void AEnemy::DealDamage(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);

	if (CurrentHealth <= 0)
	{
		bIsDead = true;
		Ragdoll();
		GetWorld()->GetTimerManager().SetTimer(
			DeadTimer,
			this,
			&AEnemy::Dead,
			5.0f,
			false
			);
	}
}

void AEnemy::Attack(AActor* Target)
{
	if (AD4_HD3_CustomProjectCharacter* Player = Cast<AD4_HD3_CustomProjectCharacter>(Target))
	{
		if (AttackMontage)
		{
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{
				AnimInstance->Montage_Play(AttackMontage);
				Player->DealDamage(DamageValue);
				bCanAttack = false;
			}
		}
	}
}

void AEnemy::Dead()
{
	Destroy();
}

