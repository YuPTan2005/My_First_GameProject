// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectFoodBTTaskNode.h"

#include "CompanionAIController.h"

EBTNodeResult::Type UCollectFoodBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* TreeComponent = &OwnerComp;
	
	if (AAIController* AIController = TreeComponent->GetAIOwner())
	{
		if (ACompanionAIController* CompanionAIController = Cast<ACompanionAIController>(AIController))
		{
			CompanionAIController->CollectFood();
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
