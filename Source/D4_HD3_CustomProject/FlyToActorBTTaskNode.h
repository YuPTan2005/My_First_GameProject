// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CompanionAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FlyToActorBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UFlyToActorBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UFlyToActorBTTaskNode();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere, Category = "Flight Settings")
	FBlackboardKeySelector AcceptanceRadius;
	
	UPROPERTY()
	AActor* TargetActor;
	
	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY()
	ACompanionAIController* AIController;
	UPROPERTY()
	ACompanion* ControlledPawn;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
