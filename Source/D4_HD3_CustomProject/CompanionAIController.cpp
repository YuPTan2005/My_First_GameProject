// Fill out your copyright notice in the Description page of Project Settings.


#include "CompanionAIController.h"

#include "BehaviorTree/BlackboardComponent.h"

void ACompanionAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACompanionAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (CompanionOwner && BlackboardComponent)
	{
		BlackboardComponent->SetValueAsVector("OwnerLocation", CompanionOwner->GetActorLocation());
	}
	
	if (TargetEnemy && BlackboardComponent && BlackboardComponent->GetValueAsBool("ChaseEnemy"))
	{
		BlackboardComponent->SetValueAsVector("EnemyLocation", TargetEnemy->GetActorLocation());
	}
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
		}
	}
	else
	{
		BlackboardComponent->SetValueAsBool("ChaseEnemy", false);
	}
}

void ACompanionAIController::UpdateAttackCheck()
{
	if (TargetEnemy)
	{
		if (FVector::Dist(ControlledCharacter->GetActorLocation(), CompanionOwner->GetActorLocation()) 
				<= ControlledCharacter->GetAttackDistance() && ControlledCharacter->CanAttack())
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

void ACompanionAIController::Attack()
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
			BlackboardComponent->SetValueAsVector("FoodLocation", 
				ControlledCharacter->GetTargetPickupFood()->GetActorLocation());
			BlackboardComponent->SetValueAsBool("GoToFoodLocation", true);
		}
		else
		{
			TargetFood = nullptr;
			BlackboardComponent->SetValueAsBool("GoToFoodLocation", false);
		}
	}
	else
	{
		BlackboardComponent->SetValueAsBool("GoToFoodLocation", false);
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

void ACompanionAIController::SetTargetEnemy(AEnemy* Enemy)
{
	TargetEnemy = Enemy;
	BlackboardComponent->SetValueAsBool("ChaseEnemy", true);
}

void ACompanionAIController::SetTargetFood(APickupFood* Food)
{
	TargetFood = Food;
	BlackboardComponent->SetValueAsBool("GoToFoodLocation", true);
}

void ACompanionAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	ControlledCharacter = Cast<ACompanion>(GetPawn());
	
	if (ControlledCharacter && ControlledCharacter->GetCompanionOwner())
	{
		CompanionOwner = ControlledCharacter->GetCompanionOwner();
	}
	
	if (RunBehaviorTree(BehaviourTree))
	{
		BlackboardComponent = GetBlackboardComponent();
		if (BlackboardComponent)
		{
			BlackboardComponent->SetValueAsBool("CanAttack", false);
			BlackboardComponent->SetValueAsBool("ChaseEnemy", false);
			BlackboardComponent->SetValueAsBool("CanCollect", false);
			BlackboardComponent->SetValueAsFloat("CollectRadius", ControlledCharacter->GetCollectRadius());
			BlackboardComponent->SetValueAsFloat("AttackRadius", ControlledCharacter->GetAttackRadius());
			BlackboardComponent->SetValueAsFloat("FollowRadius", ControlledCharacter->GetFollowRadius());
		}
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
