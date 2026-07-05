// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyToActorBTTaskNode.h"

#include "EnemyAIController.h"
#include "Kismet/KismetMathLibrary.h"

UFlyToActorBTTaskNode::UFlyToActorBTTaskNode()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
	
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UFlyToActorBTTaskNode, TargetKey), AActor::StaticClass());
	AcceptanceRadius.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UFlyToActorBTTaskNode, AcceptanceRadius));
}

void UFlyToActorBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector TargetLocation = TargetActor->GetActorLocation();
	FVector CurrentLocation = ControlledPawn->GetActorLocation();
	float Radius = BlackboardComponent->GetValueAsFloat(AcceptanceRadius.SelectedKeyName);
	
	FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	ControlledPawn->SetActorRotation(LookRotation);

	if (FVector::Dist(TargetLocation, CurrentLocation) - TargetPawnRadius - ControlledPawnRadius <= Radius)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

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
	
	if (TargetKey.SelectedKeyName.IsNone() || AcceptanceRadius.SelectedKeyName.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("TargetKey or Acceptance Radius is not set in %s"), *OwnerComp.GetName());
		return EBTNodeResult::Failed;
	}
	
	ControlledPawn = AIController->GetPawn();
	UObject* TargetObject = BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName);
	TargetActor = Cast<AActor>(TargetObject);
	if (!ControlledPawn || !TargetActor)
	{
		UE_LOG(LogTemp, Error, TEXT("ControlledPawn or TargetPawn is not set in %s"), *OwnerComp.GetName());
		return EBTNodeResult::Failed;
	}
	
	TargetPawnRadius = TargetActor->GetRootComponent()->Bounds.SphereRadius;
	ControlledPawnRadius = ControlledPawn->GetRootComponent()->Bounds.SphereRadius;
	
	return EBTNodeResult::InProgress;
}
