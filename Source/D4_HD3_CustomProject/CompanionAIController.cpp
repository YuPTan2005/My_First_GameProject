// Fill out your copyright notice in the Description page of Project Settings.


#include "CompanionAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"

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
	if (TargetEnemy)
	{
		float TargetEnemyRadius = TargetEnemy->GetCapsuleComponent()->GetScaledCapsuleRadius();
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
		ControlledCharacter->Attack(TargetEnemy);
	}
}

void ACompanionAIController::UpdateMoveToTargetFoodCheck()
{
	if (TargetFood)
	{
		if (BlackboardComponent && FVector::Dist(ControlledCharacter->GetActorLocation(), 
				TargetFood->GetActorLocation()) <= ControlledCharacter->GetCollectDistance())
		{
			BlackboardComponent->SetValueAsBool("GoToFood", true);
		}
		else
		{
			TargetFood = nullptr;
			BlackboardComponent->SetValueAsBool("GoToFood", false);
			BlackboardComponent->SetValueAsObject("Food", nullptr);
		}
	}
	else
	{
		BlackboardComponent->SetValueAsBool("GoToFood", false);
	}
}

void ACompanionAIController::UpdateCollectible()
{
	if (BlackboardComponent && ControlledCharacter->CanCollect() && !ControlledCharacter->IsCollectibleFoodListEmpty())
	{
		BlackboardComponent->SetValueAsBool("CanCollect", true);
	}
	else
	{
		BlackboardComponent->SetValueAsBool("CanCollect", false);
	}
}

void ACompanionAIController::CollectFood()
{
	ControlledCharacter->CollectFood();
}

void ACompanionAIController::SetCompanionOwner(AD4_HD3_CustomProjectCharacter* NewCompanionOwner)
{
	CompanionOwner = NewCompanionOwner;
	BlackboardComponent->SetValueAsObject("Owner", CompanionOwner);
}

void ACompanionAIController::SetTargetEnemy(AEnemy* Enemy)
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

void ACompanionAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
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
