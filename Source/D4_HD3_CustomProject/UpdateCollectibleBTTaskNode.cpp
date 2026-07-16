// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdateCollectibleBTTaskNode.h"

#include "CompanionAIController.h"

EBTNodeResult::Type UUpdateCollectibleBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* TreeComponent = &OwnerComp;
	if (AAIController* AIController = TreeComponent->GetAIOwner())
	{
		if (ACompanionAIController* CompanionAIController = Cast<ACompanionAIController>(AIController))
		{
			CompanionAIController->UpdateCollectible();
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
