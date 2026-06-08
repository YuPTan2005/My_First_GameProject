// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;
	void GenerateNewRandomLocation();
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	UPROPERTY(EditAnywhere)
	float SightRadius = 1500;
	UPROPERTY(EditAnywhere)
	float SightAge = 3.5;
	UPROPERTY(EditAnywhere)
	float LoseSightRadius = SightRadius + 50;
	UPROPERTY(EditAnywhere)
	float FieldOfView = 90;
	UPROPERTY(EditAnywhere)
	float PatrolDistance = 6000;
	UPROPERTY(EditAnywhere)
	UAISenseConfig_Sight* SightConfiguration;
	UPROPERTY(EditAnywhere)
	UBlackboardData* AIBlackboard;
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviourTree;
	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY()
	UNavigationSystemV1* NavigationSystem;
	UPROPERTY()
	APawn* TargetPlayer;
	
	void UpdateAttackCheck();
	void Attack();
	
};
