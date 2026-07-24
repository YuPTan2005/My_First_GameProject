// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SlowFallingActorComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class D4_HD3_CUSTOMPROJECT_API USlowFallingActorComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Falling Properties")
	float FallSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Falling Properties")
	float AcceptableGroundRadius = 200.0f;
	
	FTimerHandle FallTimerHandle;

	void FallingAndGroundCheck();
	
};
