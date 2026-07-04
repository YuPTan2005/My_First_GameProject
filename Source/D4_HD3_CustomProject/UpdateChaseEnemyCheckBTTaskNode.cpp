// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdateChaseEnemyCheckBTTaskNode.h"

#include "CompanionAIController.h"

void UUpdateChaseEnemyCheckBTTaskNode::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (ACompanionAIController* AIController = Cast<ACompanionAIController>(OwnerComp.GetAIOwner()))
	{
		AIController->UpdateChaseEnemyCheck();
	}
}
