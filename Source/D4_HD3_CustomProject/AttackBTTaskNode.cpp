// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBTTaskNode.h"

#include "EnemyAIController.h"

EBTNodeResult::Type UAttackBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* TreeComponent = &OwnerComp;
	AEnemyAIController* Controller = Cast<AEnemyAIController>(TreeComponent->GetOwner());
	if (Controller)
	{
		Controller->Attack();
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
