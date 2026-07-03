// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdateChaseEnemyCheckBTTaskNode.h"

#include "CompanionAIController.h"

EBTNodeResult::Type UUpdateChaseEnemyCheckBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* TreeComponent = &OwnerComp;
	ACompanionAIController* AIController = Cast<ACompanionAIController>(TreeComponent->GetOwner());
	if (AIController)
	{
		AIController->UpdateChaseEnemyCheck();
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
