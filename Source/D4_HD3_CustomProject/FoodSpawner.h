// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Food.h"
#include "PickupFood.h"
#include "GameFramework/Actor.h"
#include "FoodSpawner.generated.h"

UCLASS()
class D4_HD3_CUSTOMPROJECT_API AFoodSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoodSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	UStaticMesh* FoodMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	UMaterialInterface* FoodMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<AFood> FoodToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<APickupFood> PickupFoodClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<UFoodPickupUI> PickupUIClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SpawnSphereArea;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToSpawn = 20.0f;
	float TimePast = 0.0f;
	
	FVector GetRandomSpawnPoint();
	bool SpawnFood();
	
};
