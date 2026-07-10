// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	AEnemy* Character;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsFlying;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanAttack;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDead;
	
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
