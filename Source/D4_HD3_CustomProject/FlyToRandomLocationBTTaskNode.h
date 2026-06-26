// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FlyToRandomLocationBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UFlyToRandomLocationBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UFlyToRandomLocationBTTaskNode();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	FVector TargetLocation;
	FVector LastLocation;
	float TimeStuck;
	
	float StuckTimeOut = 0.1f;
	
};
