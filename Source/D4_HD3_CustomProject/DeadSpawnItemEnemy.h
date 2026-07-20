// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "DeadSpawnItemEnemy.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API ADeadSpawnItemEnemy : public AEnemy
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TArray<TSubclassOf<AActor>> ItemClassToSpawn;
	
public:
	virtual void Dead() override;
	
};
