// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkToPlayerBTTaskNode.h"

#include "EnemyAIController.h"

UWalkToPlayerBTTaskNode::UWalkToPlayerBTTaskNode()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UWalkToPlayerBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UWalkToPlayerBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!BlackboardComp || !AIController)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed); 
		return;
	}
	
	if (APawn* ControlledPawn = AIController->GetPawn())
	{
		FVector TargetLocation = BlackboardComp->GetValueAsVector("PlayerPosition");
		FVector CurrentLocation = ControlledPawn->GetActorLocation();
		
		FVector ResultantVector = TargetLocation - CurrentLocation;
		FVector Direction = ResultantVector.GetSafeNormal();
		
		if (ResultantVector.Z <= 200.0f)
		{
			ControlledPawn->AddMovementInput(Direction);
			
			FRotator TargetRotation(0.0f, Direction.Rotation().Yaw, 0.0f);
			ControlledPawn->SetActorRotation(TargetRotation);
			
			if (FVector::Dist(TargetLocation, CurrentLocation) <= 250.0f)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
		else
		{
			BlackboardComp->SetValueAsBool("FlyLaunch", true);
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}
}
