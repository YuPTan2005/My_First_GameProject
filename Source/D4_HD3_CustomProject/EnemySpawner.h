// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class D4_HD3_CUSTOMPROJECT_API AEnemySpawner : public AActor
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToSpawn = 60.0f;
	float TimePast = 0.0f;
	
	FVector GetRandomSpawnPoint() const;
	bool SpawnEnemy();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
