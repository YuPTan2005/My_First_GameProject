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
	UBehaviorTreeComponent* Tree = &OwnerComp;
	AEnemyAIController* Controller = Cast<AEnemyAIController>(Tree->GetOwner());
	if (Controller)
	{
		Controller->DecideMovementMode();
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
