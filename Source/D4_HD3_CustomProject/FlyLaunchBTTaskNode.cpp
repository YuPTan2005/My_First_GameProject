// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyLaunchBTTaskNode.h"

#include "EnemyAIController.h"

UFlyLaunchBTTaskNode::UFlyLaunchBTTaskNode()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UFlyLaunchBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(AIController))
		{
			EnemyAIController->OnFlying();
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
