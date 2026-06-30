// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D4_HD3_CustomProjectCharacter.h"
#include "Food.h"
#include "Banana.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API ABanana : public AFood
{
	GENERATED_BODY()
	
public:
	ABanana();
	
	virtual void EatenBy_Implementation(ACharacter* Character) override;
	
protected:
	AD4_HD3_CustomProjectCharacter* Player;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashSpeed = 4000.0f;
	
	FTimerHandle DashTimer;
	float EffectDuration = 10.0f;
	
	void RemoveDashAbility();
};
