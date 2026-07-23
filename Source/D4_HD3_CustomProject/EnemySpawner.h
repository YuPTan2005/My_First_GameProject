// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Spawner.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class D4_HD3_CUSTOMPROJECT_API AEnemySpawner : public ASpawner
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnGameStarted() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TArray<TSubclassOf<AEnemy>> AlwaysSpawnEnemyClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TArray<TSubclassOf<AEnemy>> DropHealPotionEnemyClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealPotionEnemyPercent = 0.3f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TArray<TSubclassOf<AEnemy>> RestrictedSpawnEnemyClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToSpawn = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeSpawnRestrictedEnemy = 480.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumberOfRestrictedEnemy = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RestrictedEnemySpawnPercent = 0.1f;
	float TimePast = 0.0f;
	bool bCanSpawnRestrictedEnemy;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnHealthIncreaseValue = 20.0f;
	float EnemyExtraHealth;
	
	virtual FVector GetSpawnPoint() override;
	virtual bool SpawnDefaultActor() override;
	virtual bool SpawnRestrictedEnemy();
	
	FTimerHandle RestrictedEnemyTimeTracker;
	void ToggleCanSpawnRestrictedEnemy();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
