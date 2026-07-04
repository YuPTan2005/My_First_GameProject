// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdateAttackPossibleBTTaskNode.h"

#include "EnemyAIController.h"


EBTNodeResult::Type UUpdateAttackPossibleBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController && AIController->Implements<UAttackBTInterface>())
	{
		IAttackBTInterface::Execute_UpdateAttackCheck(AIController);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
