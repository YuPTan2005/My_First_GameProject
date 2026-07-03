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
	if (TargetKey.IsSet() && AcceptanceRadius.IsSet())
	{
		FVector TargetLocation = BlackboardComponent->GetValueAsVector(TargetKey.SelectedKeyName);
		FVector CurrentLocation = ControlledPawn->GetActorLocation();
		float Radius = BlackboardComponent->GetValueAsFloat(AcceptanceRadius.SelectedKeyName);
	
		if (FVector::Dist(CurrentLocation, TargetLocation) <= Radius)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	
		FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	
		FRotator NewRotation = FMath::RInterpTo(ControlledPawn->GetActorRotation(), LookRotation, DeltaSeconds, 10.0f);
		ControlledPawn->SetActorRotation(NewRotation);
	
		const FRotator PitchYawRotation(LookRotation.Pitch, LookRotation.Yaw, 0);
		const FVector NewForwardVector = FRotationMatrix(PitchYawRotation).GetUnitAxis(EAxis::X);

		ControlledPawn->AddMovementInput(NewForwardVector);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TargetKey or Acceptance Radius is not set in FlyToActor Node in %s"), *OwnerComp.GetName());
	}
}

EBTNodeResult::Type UFlyToActorBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BlackboardComponent = OwnerComp.GetBlackboardComponent();
	AIController = Cast<ACompanionAIController>(OwnerComp.GetAIOwner());
	ControlledPawn = Cast<ACompanion>(AIController->GetPawn());
	
	if (!BlackboardComponent || !AIController || !ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::InProgress;
}
