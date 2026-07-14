// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawner.h"
#include "GameFramework/Actor.h"
#include "BagSpawner.generated.h"

UCLASS()
class D4_HD3_CUSTOMPROJECT_API ABagSpawner : public ASpawner
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABagSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual FVector GetSpawnPoint() override;
	virtual bool SpawnObject() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn")
	TSubclassOf<AActor> BagClassToSpawn;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
