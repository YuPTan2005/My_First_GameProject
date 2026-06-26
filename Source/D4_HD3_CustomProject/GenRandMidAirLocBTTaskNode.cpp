// Fill out your copyright notice in the Description page of Project Settings.


#include "GenRandMidAirLocBTTaskNode.h"

#include "EnemyAIController.h"

EBTNodeResult::Type UGenRandMidAirLocBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* TreeComponent = &OwnerComp;
	AEnemyAIController* Controller = Cast<AEnemyAIController>(TreeComponent->GetOwner());
	if (Controller)
	{
		Controller->GenerateNewRandomLocationMidAir();
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
