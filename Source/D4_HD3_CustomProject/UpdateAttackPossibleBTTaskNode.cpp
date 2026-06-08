// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdateAttackPossibleBTTaskNode.h"

#include "EnemyAIController.h"


EBTNodeResult::Type UUpdateAttackPossibleBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* TreeComponent = &OwnerComp;
	AEnemyAIController* Controller = Cast<AEnemyAIController>(TreeComponent->GetOwner());
	if (Controller)
	{
		Controller->UpdateAttackCheck();
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
