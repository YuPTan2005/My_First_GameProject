// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyToRandomLocationBTTaskNode.h"

#include "EnemyAIController.h"
#include "Kismet/KismetMathLibrary.h"

UFlyToRandomLocationBTTaskNode::UFlyToRandomLocationBTTaskNode()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

void UFlyToRandomLocationBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController || !AIController->GetPawn())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	FVector CurrentLocation = ControlledPawn->GetActorLocation();
	
	if (FVector::Dist(CurrentLocation, LastLocation) < 1.0f)
	{
		TimeStuck += DeltaSeconds;
	}
	else
	{
		TimeStuck = 0.0f;
		LastLocation = CurrentLocation;
	}
	
	if (TimeStuck >= StuckTimeOut)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	
	FRotator NewRotation = FMath::RInterpTo(ControlledPawn->GetActorRotation(), LookRotation, DeltaSeconds, 10.0f);
	ControlledPawn->SetActorRotation(NewRotation);
	
	FRotator PitchYawRotation(LookRotation.Pitch, LookRotation.Yaw, 0);
	FVector ForwardVector = FRotationMatrix(PitchYawRotation).GetUnitAxis(EAxis::X);

	ControlledPawn->AddMovementInput(ForwardVector);

	if (FVector::Dist(CurrentLocation, TargetLocation) <= 150.0f)
	{
		FRotator CurrentRotation = ControlledPawn->GetActorRotation();
		CurrentRotation.Pitch = 0.0f;
		ControlledPawn->SetActorRotation(CurrentRotation);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UFlyToRandomLocationBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;
	
	TargetLocation = BlackboardComp->GetValueAsVector("PatrolDestination");
	TimeStuck = 0.0f;
	LastLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	
	return EBTNodeResult::InProgress;
}
