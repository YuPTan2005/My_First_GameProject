// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyToPlayerBTTaskNode.h"

#include "EnemyAIController.h"
#include "Kismet/KismetMathLibrary.h"

UFlyToPlayerBTTaskNode::UFlyToPlayerBTTaskNode()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

void UFlyToPlayerBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!BlackboardComp || !AIController || !AIController->GetPawn())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed); 
		return;
	}
	
	if (APawn* ControlledPawn = AIController->GetPawn())
	{
		FVector TargetLocation = BlackboardComp->GetValueAsVector("PlayerPosition");
		
		FHitResult HitResult;
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		FCollisionShape SphereShape = FCollisionShape::MakeSphere(100);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(ControlledPawn);
	
		const FRotator Rotation = ControlledPawn->GetControlRotation();
		const FVector ForwardVector = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X) * 100.0f;
	
		bool bHitSuccess = GetWorld()->SweepSingleByObjectType(
			HitResult,
			ControlledPawn->GetActorLocation(),
			ForwardVector + ControlledPawn->GetActorLocation(),
			FQuat::Identity,
			ObjectQueryParams,
			SphereShape,
			QueryParams
			);
		
		if (bHitSuccess && HitResult.GetActor())
		{
			if (HitResult.GetActor()->ActorHasTag("Player"))
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return;
			}
		}

		FVector CurrentLocation = ControlledPawn->GetActorLocation();
		FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
		
		FRotator NewRotation = FMath::RInterpTo(ControlledPawn->GetActorRotation(), LookRotation, DeltaSeconds, 10.0f);
		ControlledPawn->SetActorRotation(NewRotation);
		
		const FRotator PitchYawRotation(LookRotation.Pitch, LookRotation.Yaw, 0);
		const FVector NewForwardVector = FRotationMatrix(PitchYawRotation).GetUnitAxis(EAxis::X);

		ControlledPawn->AddMovementInput(NewForwardVector);
	}
}

EBTNodeResult::Type UFlyToPlayerBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}
