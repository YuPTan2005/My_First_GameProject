// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateRandomLocationBTTaskNode.h"

#include "EnemyAIController.h"

EBTNodeResult::Type UGenerateRandomLocationBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* TreeComponent = &OwnerComp;
	AEnemyAIController* Controller = Cast<AEnemyAIController>(TreeComponent->GetOwner());
	if (Controller)
	{
		Controller->GenerateNewRandomLocation();
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
