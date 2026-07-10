// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AttackBTInterface.h"
#include "Companion.h"
#include "CompanionAIController.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API ACompanionAIController : public AAIController, public IAttackBTInterface
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviourTree;
	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;
	
	void UpdateChaseEnemyCheck();
	virtual void UpdateAttackCheck_Implementation() override;
	virtual void Attack_Implementation() override;
	
	void UpdateMoveToTargetFoodCheck();
	void UpdateCollectible();
	void CollectFood() const;
	
	void SetCompanionOwner(AActor* NewCompanionOwner);
	void SetTargetEnemy(AActor* Enemy);
	void SetTargetFood(APickupFood* Food);
	
	void ClearFoodTarget() const;
	
protected:
	UPROPERTY()
	ACompanion* ControlledCharacter;
	UPROPERTY()
	AActor* CompanionOwner;
	UPROPERTY()
	AActor* TargetEnemy;
	UPROPERTY()
	APickupFood* TargetFood;
	
	virtual void OnPossess(APawn* InPawn) override;
	
	virtual FRotator GetControlRotation() const override;
	
};
