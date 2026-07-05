// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyAIController.h"

#include "BehaviorTree/BTTaskNode.h"
#include "WalkToPlayerBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UWalkToPlayerBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UWalkToPlayerBTTaskNode();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere, Category = "Flight Settings")
	FBlackboardKeySelector AcceptanceRadius;
	
	float RetrievedAcceptanceRadius;
	
	UPROPERTY()
	APawn* TargetPawn;
	
	UPROPERTY()
	AAIController* AIController;
	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY()
	APawn* ControlledPawn;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
