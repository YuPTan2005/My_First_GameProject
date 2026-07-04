// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBTTaskNode.h"

#include "EnemyAIController.h"

EBTNodeResult::Type UAttackBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController && AIController->Implements<UAttackBTInterface>())
	{
		IAttackBTInterface::Execute_Attack(AIController);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
