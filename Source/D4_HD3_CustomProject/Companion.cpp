// Fill out your copyright notice in the Description page of Project Settings.


#include "Companion.h"
#include "BrainComponent.h"
#include "AIController.h"
#include "CompanionAIController.h"
#include "D4_HD3_CustomProjectCharacter.h"
#include "Enemy.h"
#include "Food.h"
#include "NPCStatus.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACompanion::ACompanion()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCharacterMovement()->NavAgentProps.bCanFly = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Food detect area"));
	SphereComponent->SetupAttachment(RootComponent);
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACompanion::OnSphereOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ACompanion::OnSphereEndOverlap);
}

// Called when the game starts or when spawned
void ACompanion::BeginPlay()
{
	Super::BeginPlay();
	
	bIsDead = false;
	bCanAttack = true;
	bCanCollect = true;
	
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	if (SphereComponent)
	{
		SphereComponent->SetSphereRadius(CollectRadius);
	}
	
	if (CompanionStatusClass)
	{
		StatusWidget = CreateWidget<UNPCStatus>(GetWorld(), CompanionStatusClass);
		if (StatusWidget)
		{
			StatusWidget->BindingActor = this;
		
			if (StatusComponent)
			{
				StatusComponent->SetWidget(StatusWidget);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("StatusComponent is null in %s"), *GetName());
			}
			UpdateStatus();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("StatusWidget is null in %s"), *GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CompanionStatusClass is null in %s"), *GetName());
	}
}

void ACompanion::OnSphereOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APickupFood* PickupFood = Cast<APickupFood>(OtherActor))
	{
		PickupFoodList.AddUnique(PickupFood);
		SelectNextFoodTarget();
	}
}

void ACompanion::OnSphereEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (APickupFood* PickupFood = Cast<APickupFood>(OtherActor))
	{
		if (TargetPickupFood == PickupFood)
		{
			TargetPickupFood = nullptr;
			Cast<ACompanionAIController>(GetController())->ClearFoodTarget();
			SelectNextFoodTarget();
		}
		PickupFoodList.Remove(PickupFood);
	}
}

void ACompanion::Dead()
{
	Destroy();
}

void ACompanion::DealDamage_Implementation(float DamageTook, AActor* DamagedBy)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageTook, 0.0f, MaxHealth);
	UpdateStatus();
	
	if (AEnemy* DamagedByEnemy = Cast<AEnemy>(DamagedBy))
	{
		SetTargetEnemy(DamagedByEnemy);
	}
	
	if (CurrentHealth <= 0 && !bIsDead)
	{
		bIsDead = true;
		
		if (AAIController* AIController = Cast<AAIController>(GetController()))
		{
			UBrainComponent* AIBrainComponent = AIController->GetBrainComponent();
			if (AIBrainComponent && AIBrainComponent->IsRunning())
			{
				AIBrainComponent->StopLogic(TEXT("Character died"));
			}
		}
		GetWorld()->GetTimerManager().SetTimer(
			DeadTimer,
			this,
			&ACompanion::Dead,
			5.0f,
			false
			);
	}
}

EGameTeam ACompanion::GetTeam_Implementation()
{
	return EGameTeam::Players;
}

void ACompanion::Attack(AActor* Target)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (AttackMontage)
		{
			AnimInstance->Montage_Play(AttackMontage);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s doesn't have attack montage"), *GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s doesn't have anim instance"), *GetName());
	}
	
	if (Target->Implements<UDamageable>())
	{
		Execute_DealDamage(Target, DamageValue, this);
		AttackTimer = 0.0f;
	}
}

bool ACompanion::CollectFood()
{
	if (!TargetPickupFood)
	{
		return false;
	}
	
	if (CollectibleFoodList.Contains(TargetPickupFood))
	{
		APickupFood* PickupFood = TargetPickupFood;
		AFood* FoodToCollect = nullptr;
       
		if (!PickupFood->Food)
		{
			FoodToCollect = NewObject<AFood>();
		}
		else
		{
			FoodToCollect = PickupFood->Food;
		}
       
		if (CompanionOwner && CompanionOwner->AddItem(FoodToCollect))
		{
			CollectibleFoodList.RemoveSingle(PickupFood);
			PickupFoodList.RemoveSingle(PickupFood);
			
			CompanionOwner->RemoveCollectibleFood_Implementation(PickupFood);
			
			CollectTimer = 0.0f;
			TargetPickupFood = nullptr; 
			
			PickupFood->Collected();
			SelectNextFoodTarget();
			
			return true;
		}
	}
    
	return false;
}

void ACompanion::AddCollectibleFood_Implementation(APickupFood* Food)
{
	CollectibleFoodList.Add(Food);
}

void ACompanion::RemoveCollectibleFood_Implementation(APickupFood* Food)
{
	CollectibleFoodList.RemoveSingle(Food);
}

bool ACompanion::IsCollectibleFoodListEmpty()
{
	return CollectibleFoodList.IsEmpty();
}

void ACompanion::SelectNextFoodTarget()
{
	if (!TargetPickupFood && !PickupFoodList.IsEmpty())
	{
		TargetPickupFood = PickupFoodList[0];
		if (ACompanionAIController* AIController = Cast<ACompanionAIController>(GetController()))
		{
			AIController->SetTargetFood(TargetPickupFood);
		}
	}
}

void ACompanion::UpdateStatus()
{
	if (StatusWidget)
	{
		StatusWidget->UpdateValues();
	}
}

float ACompanion::GetCurrentHealth_Implementation()
{
	return CurrentHealth;
}

float ACompanion::GetMaxHealth_Implementation()
{
	return MaxHealth;
}

float ACompanion::GetDamageValue() const
{
	return DamageValue;
}

float ACompanion::GetStarvationValue() const
{
	return StarvationValue;
}

float ACompanion::GetMaxStarvationValue() const
{
	return MaxStarvationValue;
}

float ACompanion::GetStarvationDecrementValue() const
{
	return StarvationDecrementValue;
}

bool ACompanion::IsDead_Implementation()
{
	return bIsDead;
}

bool ACompanion::CanAttack() const
{
	return bCanAttack;
}

bool ACompanion::CanCollect() const
{
	return bCanCollect;
}

AD4_HD3_CustomProjectCharacter* ACompanion::GetCompanionOwner()
{
	return CompanionOwner;
}

bool ACompanion::IsPickupFoodListEmpty()
{
	return PickupFoodList.IsEmpty();
}

float ACompanion::GetFollowRadius() const
{
	return FollowRadius;
}

void ACompanion::SetFollowRadius(float NewFollowRadius)
{
	this->FollowRadius = NewFollowRadius;
}

float ACompanion::GetAttackRadius() const
{
	return AttackRadius;
}

void ACompanion::SetAttackRadius(float NewAttackRadius)
{
	this->AttackRadius = NewAttackRadius;
}

float ACompanion::GetAttackDistance() const
{
	return AttackDistance;
}

void ACompanion::SetAttackDistance(float NewAttackDistance)
{
	this->AttackDistance = NewAttackDistance;
}

float ACompanion::GetCollectRadius() const
{
	return CollectRadius;
}

void ACompanion::SetCollectRadius(float NewCollectRadius)
{
	this->CollectRadius = NewCollectRadius;
}

float ACompanion::GetCollectDistance() const
{
	return CollectDistance;
}

void ACompanion::SetCollectDistance(float NewCollectDistance)
{
	this->CollectDistance = NewCollectDistance;
}

APickupFood* ACompanion::GetTargetPickupFood()
{
	return TargetPickupFood;
}

AEnemy* ACompanion::GetTargetEnemy()
{
	return TargetEnemy;
}

void ACompanion::SetTargetEnemy(AEnemy* Enemy)
{
	TargetEnemy = Enemy;
	if (ACompanionAIController* AIController = Cast<ACompanionAIController>(GetController()))
	{
		AIController->SetTargetEnemy(Enemy);
	}
}

void ACompanion::SetTargetPickupFood(APickupFood* NewTargetPickupFood)
{
	TargetPickupFood = NewTargetPickupFood;
}

void ACompanion::SetCurrentHealth(float NewCurrentHealth)
{
	this->CurrentHealth = NewCurrentHealth;
}

void ACompanion::SetMaxHealth(float NewMaxHealth)
{
	this->MaxHealth = NewMaxHealth;
}

void ACompanion::SetDamageValue(float NewDamageValue)
{
	this->DamageValue = NewDamageValue;
}

void ACompanion::SetStarvationValue(float NewMaxStarvationValue)
{
	this->StarvationValue = NewMaxStarvationValue;
}

void ACompanion::SetMaxStarvationValue(float NewMaxStarvationValue)
{
	this->MaxStarvationValue = NewMaxStarvationValue;
}

void ACompanion::SetStarvationDecrementValue(float NewStarvationDecrementValue)
{
	this->StarvationDecrementValue = NewStarvationDecrementValue;
}

void ACompanion::SetIsDead(bool NewValue)
{
	this->bIsDead = NewValue;
}

void ACompanion::SetCanAttack(bool NewValue)
{
	this->bCanAttack = NewValue;
}

void ACompanion::SetCanCollect(bool NewValue)
{
	this->bCanCollect = NewValue;
}

void ACompanion::SetCompanionOwner(AD4_HD3_CustomProjectCharacter* NewCompanionOwner)
{
	CompanionOwner = NewCompanionOwner;
	
	// Align the flying speed of companion with owner
	GetCharacterMovement()->MaxFlySpeed = CompanionOwner->GetCharacterMovement()->MaxFlySpeed;
	GetCharacterMovement()->BrakingDecelerationFlying = CompanionOwner->GetCharacterMovement()->BrakingDecelerationFlying;
	
	if (ACompanionAIController* AIController = Cast<ACompanionAIController>(GetController()))
	{
		AIController->SetCompanionOwner(CompanionOwner);
	}
}

// Called every frame
void ACompanion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CollectTimer >= CollectInterval)
	{
		bCanCollect = true;
	}
	else
	{
		CollectTimer += DeltaTime;
		bCanCollect = false;
	}
	
	if (AttackTimer >= AttackInterval)
	{
		bCanAttack = true;
	}
	else
	{
		AttackTimer += DeltaTime;
		bCanAttack = false;
	}
}

// Called to bind functionality to input
void ACompanion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

