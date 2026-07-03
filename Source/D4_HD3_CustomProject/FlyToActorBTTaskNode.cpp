// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyToActorBTTaskNode.h"

#include "EnemyAIController.h"
#include "Kismet/KismetMathLibrary.h"

UFlyToActorBTTaskNode::UFlyToActorBTTaskNode()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
	
	TargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UFlyToActorBTTaskNode, TargetKey));
	AcceptanceRadius.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UFlyToActorBTTaskNode, AcceptanceRadius));
}

void UFlyToActorBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector TargetLocation = BlackboardComponent->GetValueAsVector(TargetKey.SelectedKeyName);
	FVector CurrentLocation = ControlledPawn->GetActorLocation();
	float Radius = BlackboardComponent->GetValueAsFloat(AcceptanceRadius.SelectedKeyName);

	if (FVector::Dist(TargetLocation, CurrentLocation) <= Radius)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	FRotator NewRotation = FMath::RInterpTo(ControlledPawn->GetActorRotation(), LookRotation, DeltaSeconds, 10.0f);
	ControlledPawn->SetActorRotation(NewRotation);

	FVector MoveDirection = ControlledPawn->GetActorForwardVector();
	ControlledPawn->AddMovementInput(MoveDirection, 1.0f);
}

EBTNodeResult::Type UFlyToActorBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BlackboardComponent = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (!BlackboardComponent || !AIController)
	{
		return EBTNodeResult::Failed;
	}
	
	ControlledPawn = Cast<ACompanion>(AIController->GetPawn());
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	if (TargetKey.SelectedKeyName.IsNone() || AcceptanceRadius.SelectedKeyName.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("TargetKey or Acceptance Radius is not set in FlyToActor Node in %s"), *OwnerComp.GetName());
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::InProgress;
}
