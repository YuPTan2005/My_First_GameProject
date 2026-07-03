// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdateMoveToTargetFoodBTTaskNode.h"

#include "CompanionAIController.h"

EBTNodeResult::Type UUpdateMoveToTargetFoodBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* TreeComponent = &OwnerComp;
	ACompanionAIController* AIController = Cast<ACompanionAIController>(TreeComponent->GetOwner());
	if (AIController)
	{
		AIController->UpdateMoveToTargetFoodCheck();
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
