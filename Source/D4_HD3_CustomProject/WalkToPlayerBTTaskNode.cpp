// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkToPlayerBTTaskNode.h"

#include "EnemyAIController.h"
#include "Components/CapsuleComponent.h"
#include "Navigation/PathFollowingComponent.h"

UWalkToPlayerBTTaskNode::UWalkToPlayerBTTaskNode()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UWalkToPlayerBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BlackboardComp = OwnerComp.GetBlackboardComponent();
	AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	ControlledPawn = Cast<AEnemy>(AIController->GetPawn());
	
	if (!BlackboardComp || !AIController || !ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	if (ACharacter* Target = Cast<ACharacter>(AIController->TargetPlayer))
	{
		float TargetRadius = Target->GetCapsuleComponent()->GetScaledCapsuleRadius();
		float PawnRadius = ControlledPawn->GetCapsuleComponent()->GetScaledCapsuleRadius();
		AcceptanceRadius = ControlledPawn->AttackDistance - TargetRadius - PawnRadius;
	}
	
	return EBTNodeResult::InProgress;
}

void UWalkToPlayerBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector TargetLocation = BlackboardComp->GetValueAsVector("PlayerPosition");
	FVector CurrentLocation = ControlledPawn->GetActorLocation();
	
	if (FMath::Abs(TargetLocation.Z - CurrentLocation.Z) > 300.0f)
	{
		BlackboardComp->SetValueAsBool("FlyLaunch", true);
		AIController->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(TargetLocation, AcceptanceRadius, true, true, true, false);

	if (MoveResult == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		AIController->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else if (MoveResult == EPathFollowingRequestResult::Type::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
