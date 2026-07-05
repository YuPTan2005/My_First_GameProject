// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkToPlayerBTTaskNode.h"

#include "EnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"

UWalkToPlayerBTTaskNode::UWalkToPlayerBTTaskNode()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
	
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UWalkToPlayerBTTaskNode, TargetKey), AActor::StaticClass());
	AcceptanceRadius.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UWalkToPlayerBTTaskNode, AcceptanceRadius));
}

EBTNodeResult::Type UWalkToPlayerBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BlackboardComponent = OwnerComp.GetBlackboardComponent();
	AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (!BlackboardComponent || !AIController)
	{
		return EBTNodeResult::Failed;
	}
	
	if (TargetKey.SelectedKeyName.IsNone() || AcceptanceRadius.SelectedKeyName.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("TargetKey or Acceptance Radius is not set in WalkToPlayer Node in %s"), *OwnerComp.GetName());
		return EBTNodeResult::Failed;
	}
	
	RetrievedAcceptanceRadius = BlackboardComponent->GetValueAsFloat(AcceptanceRadius.SelectedKeyName);
	ControlledPawn = AIController->GetPawn();
	UObject* TargetObject = BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName);
	TargetPawn = Cast<APawn>(TargetObject);
	if (!ControlledPawn || !TargetPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	FVector CurrentLocation = ControlledPawn->GetActorLocation();
	FVector TargetLocation = TargetPawn->GetActorLocation();
	if (FMath::Abs(TargetLocation.Z - CurrentLocation.Z) > 500.0f)
	{
		BlackboardComponent->SetValueAsBool("FlyLaunch", true);
		return EBTNodeResult::Failed; 
	}
	
	EPathFollowingRequestResult::Type MoveResult = AIController->MoveToActor(
		TargetPawn, 
		RetrievedAcceptanceRadius, 
		true,
		true,
		true,
		nullptr, 
		false
	);

	if (MoveResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		return EBTNodeResult::Succeeded;
	}
	if (MoveResult == EPathFollowingRequestResult::Failed)
	{
		return EBTNodeResult::Failed;
	}
    
	return EBTNodeResult::InProgress;
}

void UWalkToPlayerBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector CurrentLocation = ControlledPawn->GetActorLocation();
	FVector TargetLocation = TargetPawn->GetActorLocation();
	
	if (FMath::Abs(TargetLocation.Z - CurrentLocation.Z) > 350.0f)
	{
		BlackboardComponent->SetValueAsBool("FlyLaunch", true);
		AIController->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	UPathFollowingComponent* PathFollowComp = AIController->GetPathFollowingComponent();
	if (PathFollowComp && PathFollowComp->DidMoveReachGoal())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
