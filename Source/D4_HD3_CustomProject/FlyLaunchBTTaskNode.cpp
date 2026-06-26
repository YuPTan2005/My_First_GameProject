// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyLaunchBTTaskNode.h"

#include "EnemyAIController.h"

UFlyLaunchBTTaskNode::UFlyLaunchBTTaskNode()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UFlyLaunchBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		AIController->OnFlying();
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
