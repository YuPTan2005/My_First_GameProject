// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeadSpawnItemEnemy.h"
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TArray<TSubclassOf<AEnemy>> EnemyClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TArray<TSubclassOf<ADeadSpawnItemEnemy>> DeadSpawnItemEnemyClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToSpawn = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeSpawnDropItemEnemy = 480.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumberOfDropItemEnemy = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropItemEnemySpawnPercent = 0.1f;
	float TimePast = 0.0f;
	bool bCanSpawnDropItemEnemy;
	
	virtual FVector GetSpawnPoint() override;
	virtual bool SpawnObject() override;
	virtual bool SpawnDropItemEnemy();
	
	FTimerHandle DropItemEnemyTimeTracker;
	void ToggleCanSpawnDropItemEnemy();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
