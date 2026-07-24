// Fill out your copyright notice in the Description page of Project Settings.


#include "CompanionAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

void ACompanionAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACompanionAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void ACompanionAIController::UpdateChaseEnemyCheck()
{
	if (TargetEnemy && CompanionOwner)
	{
		if (BlackboardComponent && FVector::Dist(TargetEnemy->GetActorLocation(), 
				CompanionOwner->GetActorLocation()) <= ControlledCharacter->GetAttackRadius())
		{
			BlackboardComponent->SetValueAsBool("ChaseEnemy", true);
		}
		else
		{
			TargetEnemy = nullptr;
			BlackboardComponent->SetValueAsBool("ChaseEnemy", false);
			BlackboardComponent->SetValueAsObject("Enemy", nullptr);
		}
	}
	else
	{
		BlackboardComponent->SetValueAsBool("ChaseEnemy", false);
	}
}

void ACompanionAIController::UpdateAttackCheck_Implementation()
{
	if (TargetEnemy && BlackboardComponent)
	{
		float TargetEnemyRadius = TargetEnemy->GetRootComponent()->Bounds.SphereRadius;
		float ControlledCharacterRadius = ControlledCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
		
		if (FVector::Dist(TargetEnemy->GetActorLocation(), ControlledCharacter->GetActorLocation()) - 
				TargetEnemyRadius - ControlledCharacterRadius <= ControlledCharacter->GetAttackDistance() 
			&& ControlledCharacter->CanAttack())
		{
			BlackboardComponent->SetValueAsBool("CanAttack", true);
		}
		else
		{
			BlackboardComponent->SetValueAsBool("CanAttack", false);
		}
	}
	else
	{
		BlackboardComponent->SetValueAsBool("CanAttack", false);
	}
}

void ACompanionAIController::Attack_Implementation()
{
	if (TargetEnemy && ControlledCharacter)
	{
		FVector TargetLocation = TargetEnemy->GetActorLocation();
		FVector CurrentLocation = ControlledCharacter->GetActorLocation();
	
		FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
		ControlledCharacter->SetActorRotation(LookRotation);
		
		ControlledCharacter->Attack(TargetEnemy);
	}
}

void ACompanionAIController::UpdateMoveToTargetFoodCheck()
{
	if (!BlackboardComponent || !CompanionOwner || !ControlledCharacter) return;

	FVector CompanionLocation = ControlledCharacter->GetActorLocation();
	FVector OwnerLocation = CompanionOwner->GetActorLocation();
    
	float DistanceToOwner = FVector::Dist(CompanionLocation, OwnerLocation);
	float CollectRadius = ControlledCharacter->GetCollectRadius();

	if (DistanceToOwner > CollectRadius)
	{
		ClearFoodTarget();
		return; 
	}

	if (!TargetFood)
	{
		ControlledCharacter->SelectNextFoodTarget();
	}

	if (TargetFood)
	{
		BlackboardComponent->SetValueAsBool("GoToFood", true);
	}
}

void ACompanionAIController::UpdateCollectible()
{
	if (TargetFood && BlackboardComponent && 
		CompanionOwner->Implements<UDamageable>() && !IDamageable::Execute_IsDead(CompanionOwner))
	{
		float TargetFoodRadius = TargetFood->GetRootComponent()->Bounds.SphereRadius;
		float ControlledCharacterRadius = ControlledCharacter->GetRootComponent()->Bounds.SphereRadius;
		
		if (!ControlledCharacter->IsCollectibleFoodListEmpty() && ControlledCharacter->CanCollect()
			&& FVector::Dist(TargetFood->GetActorLocation(), ControlledCharacter->GetActorLocation()) -
				TargetFoodRadius - ControlledCharacterRadius <= ControlledCharacter->GetCollectDistance())
		{
			BlackboardComponent->SetValueAsBool("CanCollect", true);
		}
		else
		{
			BlackboardComponent->SetValueAsBool("CanCollect", false);
		}
	}
	else
	{
		ClearFoodTarget();
	}
}

void ACompanionAIController::CollectFood() const
{
	ControlledCharacter->CollectFood();
}

void ACompanionAIController::SetCompanionOwner(AActor* NewCompanionOwner)
{
	CompanionOwner = NewCompanionOwner;
	BlackboardComponent->SetValueAsObject("Owner", CompanionOwner);
}

void ACompanionAIController::SetTargetEnemy(AActor* Enemy)
{
	TargetEnemy = Enemy;
	BlackboardComponent->SetValueAsBool("ChaseEnemy", true);
	BlackboardComponent->SetValueAsObject("Enemy", Enemy);
}

void ACompanionAIController::SetTargetFood(APickupFood* Food)
{
	TargetFood = Food;
	BlackboardComponent->SetValueAsBool("GoToFood", true);
	BlackboardComponent->SetValueAsObject("Food", TargetFood);
}

void ACompanionAIController::ClearFoodTarget()
{
	if (BlackboardComponent && ControlledCharacter)
	{
		BlackboardComponent->SetValueAsBool("GoToFood", false);
		BlackboardComponent->SetValueAsBool("CanCollect", false);
		BlackboardComponent->SetValueAsObject("Food", nullptr);
		ControlledCharacter->SetTargetPickupFood(nullptr);
		TargetFood = nullptr;
	}
}

void ACompanionAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (!InPawn) return;
	ControlledCharacter = Cast<ACompanion>(InPawn);
	
	if (ControlledCharacter && BehaviourTree)
	{
		if (RunBehaviorTree(BehaviourTree))
		{
			BlackboardComponent = GetBlackboardComponent();
			if (BlackboardComponent)
			{
				BlackboardComponent->SetValueAsBool("CanAttack", false);
				BlackboardComponent->SetValueAsBool("ChaseEnemy", false);
				BlackboardComponent->SetValueAsBool("CanCollect", false);
				BlackboardComponent->SetValueAsFloat("CollectDistance", ControlledCharacter->GetCollectDistance());
				BlackboardComponent->SetValueAsFloat("AttackDistance", ControlledCharacter->GetAttackDistance());
				BlackboardComponent->SetValueAsFloat("FollowDistance", ControlledCharacter->GetFollowRadius());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Couldn't get blackboard component in %s"), *GetName())
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviourTree is not set in %s"), *GetName())
	}
}

FRotator ACompanionAIController::GetControlRotation() const
{
	if (GetPawn())
	{
		FRotator PawnRotation = GetPawn()->GetActorRotation();
		return FRotator(PawnRotation.Pitch, PawnRotation.Yaw, 0);
	}
	return Super::GetControlRotation();
}
