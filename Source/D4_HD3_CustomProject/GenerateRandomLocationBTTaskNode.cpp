// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateRandomLocationBTTaskNode.h"

#include "EnemyAIController.h"

EBTNodeResult::Type UGenerateRandomLocationBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* TreeComponent = &OwnerComp;
	if (AAIController* AIController = TreeComponent->GetAIOwner())
	{
		if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(AIController))
		{
			EnemyAIController->GenerateNewRandomLocationLand();
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
