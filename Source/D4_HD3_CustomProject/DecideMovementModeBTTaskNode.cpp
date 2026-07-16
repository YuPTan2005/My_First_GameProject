// Fill out your copyright notice in the Description page of Project Settings.


#include "DecideMovementModeBTTaskNode.h"

#include "EnemyAIController.h"

UDecideMovementModeBTTaskNode::UDecideMovementModeBTTaskNode()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UDecideMovementModeBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(AIController))
		{
			EnemyAIController->DecideMovementMode();
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
