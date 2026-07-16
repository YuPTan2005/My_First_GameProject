// Fill out your copyright notice in the Description page of Project Settings.


#include "GenRandMidAirLocBTTaskNode.h"

#include "EnemyAIController.h"

EBTNodeResult::Type UGenRandMidAirLocBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(AIController);)
		{
			EnemyAIController->GenerateNewRandomLocationMidAir();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
