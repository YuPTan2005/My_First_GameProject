// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AttackBTInterface.h"
#include "Enemy.h"
#include "NavigationSystem.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API AEnemyAIController : public AAIController, public IAttackBTInterface
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;
	
	UPROPERTY()
	AEnemy* ControlledCharacter;
	
	void OnLanding();
	void OnFlying();
	void DecideMovementMode();
	void GenerateNewRandomLocationLand();
	void GenerateNewRandomLocationMidAir();
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	UPROPERTY(EditAnywhere)
	float SightRadius = 1500;
	UPROPERTY(EditAnywhere)
	float SightAge = 3.5;
	UPROPERTY(EditAnywhere)
	float LoseSightRadius = SightRadius + 100;
	UPROPERTY(EditAnywhere)
	float FieldOfView = 135;
	UPROPERTY(EditAnywhere)
	float PatrolDistance = 6000;
	UPROPERTY(EditAnywhere)
	float MaxPatrolHeight = 2200.0f;
	UPROPERTY(EditAnywhere)
	UAISenseConfig_Sight* SightConfiguration;
	UPROPERTY(EditAnywhere)
	UBlackboardData* AIBlackboard;
	UPROPERTY(EditAnywhere)
	UBehaviorTree* WalkBehaviourTree;
	UPROPERTY(EditAnywhere)
	UBehaviorTree* FlyBehaviourTree;
	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY()
	UNavigationSystemV1* NavigationSystem;
	UPROPERTY()
	APawn* TargetPlayer;
	
	float EnemyOriginalWalkSpeed = 300;
	float EnemyOriginalFlySpeed = 450;
	float EnemyIncreasedWalkSpeed = 500;
	float EnemyIncreasedFlySpeed = 600;
	
	virtual void UpdateAttackCheck_Implementation() override;
	virtual void Attack_Implementation() override;
	
};
