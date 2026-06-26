// Fill out your copyright notice in the Description page of Project Settings.


#include "LandingBTTaskNode.h"

#include "EnemyAIController.h"

ULandingBTTaskNode::ULandingBTTaskNode()
{
	bNotifyTick = true;
}

EBTNodeResult::Type ULandingBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void ULandingBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if (APawn* ControlledPawn = AIController->GetPawn())
		{
			FVector Start = ControlledPawn->GetActorLocation();
			FVector End = Start + FVector(0.0f, 0.0f, -300.0f);

			FHitResult HitResult;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(ControlledPawn);
			
			bool bHit = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				Start,
				End,
				ECC_WorldStatic,
				QueryParams
			);
			
			if (bHit)
			{
				UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
				if (!BlackboardComponent)
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				}
				BlackboardComponent->SetValueAsBool("Landing", false);
				AIController->OnLanding();
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
			else
			{
				float CurrentYaw = ControlledPawn->GetActorRotation().Yaw;
				float Angles[] = { 0.0f, 45.0f, -45.0f, 90.0f, -90.0f, 135.0f, -135.0f, 180.0f };
    
				for (float Angle : Angles)
				{
					FRotator CheckRotation(-65.0f, CurrentYaw + Angle, 0.0f);
					FVector Direction = CheckRotation.Vector();

					if (IsDirectionClear(ControlledPawn, Direction))
					{
						ControlledPawn->AddMovementInput(Direction);
						break;
					}
				}
			}
		}
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

bool ULandingBTTaskNode::IsDirectionClear(APawn* Pawn, FVector Direction)
{
	float CheckDistance = 150.0f;
	FVector Start = Pawn->GetActorLocation();
	FVector End = Start + (Direction * CheckDistance);
    
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Pawn);
	
	return !GetWorld()->SweepSingleByChannel(
		Hit, 
		Start, 
		End, 
		FQuat::Identity, 
		ECC_WorldStatic, 
		FCollisionShape::MakeSphere(45.0f),
		Params
		);
}
