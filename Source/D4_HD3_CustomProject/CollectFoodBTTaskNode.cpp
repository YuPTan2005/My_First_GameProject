// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectFoodBTTaskNode.h"

#include "CompanionAIController.h"

EBTNodeResult::Type UCollectFoodBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* TreeComponent = &OwnerComp;
	ACompanionAIController* AIController = Cast<ACompanionAIController>(TreeComponent->GetOwner());
	if (AIController)
	{
		AIController->CollectFood();
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
