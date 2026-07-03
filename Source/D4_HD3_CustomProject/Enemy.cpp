// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "BrainComponent.h"
#include "D4_HD3_CustomProjectCharacter.h"
#include "EnemyAIController.h"
#include "EnemyStatus.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCharacterMovement()->MaxWalkSpeed = 300;
	
	StatusComponent = CreateDefaultSubobject<UEnemyStatusComponent>(TEXT("Health Bar Component"));
	StatusComponent->SetupAttachment(GetMesh(), FName("head"));
	
	StatusComponent->SetWidgetSpace(EWidgetSpace::Screen);
	StatusComponent->SetDrawSize(FVector2D(200.0f, 20.0f));
	
	GetCharacterMovement()->NavAgentProps.bCanFly = true;
	this->GetCharacterMovement()->BrakingDecelerationFlying = 2000;
	this->GetCharacterMovement()->MaxFlySpeed = 450;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	bCanAttack = false;
	bIsFlying = false;
	bIsDead = false;
	
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

void AEnemy::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	
	if (PrevMovementMode == MOVE_Flying && GetCharacterMovement()->MovementMode == MOVE_Walking)
	{
		FRotator CurrentRotation = GetActorRotation();
		CurrentRotation.Pitch = 0.0f;
		SetActorRotation(CurrentRotation);

		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void AEnemy::DealDamage_Implementation(float DamageTaken, AActor* DamagedBy)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageTaken, 0.0f, MaxHealth);
	UpdateStatus();
	
	if (CurrentHealth <= 0 && !bIsDead)
	{
		bIsDead = true;
		
		if (AAIController* AIController = Cast<AAIController>(GetController()))
		{
			UBrainComponent* AIBrainComponent = AIController->GetBrainComponent();
			if (AIBrainComponent && AIBrainComponent->IsRunning())
			{
				AIBrainComponent->StopLogic(TEXT("Character Dead"));
			}
		}
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

void AEnemy::DisplayFlyLaunchMontage()
{
	if (FlyLaunchMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(FlyLaunchMontage);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ControlledCharacter doesn't have anim instance"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FlyLaunchMontage in EnemyAIController hasn't been assigned"));
	}
}

void AEnemy::DisplayLandingMontage()
{
	if (LandingMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(LandingMontage);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ControlledCharacter doesn't have anim instance"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LandingMontage in EnemyAIController hasn't been assigned"));
	}
}

void AEnemy::Attack(AActor* Target)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (GetCharacterMovement()->MovementMode == MOVE_Walking && WalkAttackMontage)
		{
			AnimInstance->Montage_Play(WalkAttackMontage);
		}
		else if (GetCharacterMovement()->MovementMode == MOVE_Flying && FlyAttackMontage)
		{
			AnimInstance->Montage_Play(FlyAttackMontage);
		}
	}
	
	if (Target->Implements<UDamageable>())
	{
		Execute_DealDamage(Target, DamageValue, this);
		bCanAttack = false;
	}
}

void AEnemy::Dead()
{
	Destroy();
}

