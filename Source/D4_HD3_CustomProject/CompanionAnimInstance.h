// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Companion.h"
#include "Animation/AnimInstance.h"
#include "CompanionAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UCompanionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	ACompanion* Companion;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanAttack;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDead;
	
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
