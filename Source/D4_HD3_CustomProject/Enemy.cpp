// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "BrainComponent.h"
#include "D4_HD3_CustomProjectCharacter.h"
#include "EnemyAIController.h"
#include "EnemyStatus.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanAttack = false;
	bIsDead = false;
	
	GetCharacterMovement()->MaxWalkSpeed = 400;
	
	StatusComponent = CreateDefaultSubobject<UEnemyStatusComponent>(TEXT("Health Bar Component"));
	StatusComponent->SetupAttachment(GetMesh(), FName("head"));
	
	StatusComponent->SetWidgetSpace(EWidgetSpace::Screen);
	StatusComponent->SetDrawSize(FVector2D(200.0f, 20.0f));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	StatusWidget = CreateWidget<UEnemyStatus>(GetWorld(), EnemyStatusClass);
	if (StatusWidget)
	{
		StatusWidget->Player = this;
		
		if (StatusComponent)
		{
			StatusComponent->SetWidget(StatusWidget);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("StatusComponent is null"));
		}
		UpdateStatus();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StatusWidget is null"));
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (StatusWidget)
	{
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (CameraManager)
		{
			float Distance = FVector::Dist(CameraManager->GetCameraLocation(), GetActorLocation());
			
			float ReferenceDistance = 1200.0f;
			float MinimumSafeDistance = 50.0f;
			float SafeDistance = FMath::Max(Distance, MinimumSafeDistance);
			
			float TargetScale = ReferenceDistance / SafeDistance;
			
			StatusWidget->SetRenderScale(FVector2D(TargetScale, TargetScale));
		}
	}
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
	UpdateStatus();
	
	if (CurrentHealth <= 0 && !bIsDead)
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

void AEnemy::UpdateStatus()
{
	if (StatusWidget)
	{
		StatusWidget->UpdateValues();
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
	Ragdoll();
	Destroy();
}

