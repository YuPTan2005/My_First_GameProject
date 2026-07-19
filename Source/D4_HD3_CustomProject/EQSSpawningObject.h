// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Food.h"
#include "PickupUI.h"
#include "PickupFood.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/Actor.h"
#include "EQSSpawningObject.generated.h"

UCLASS()
class D4_HD3_CUSTOMPROJECT_API AEQSSpawningObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEQSSpawningObject();
	
	UPROPERTY(EditAnywhere, Category = "EQS")
	UEnvQuery* SpawnQuery;

	void TriggerSpawnLocationQuery();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void OnSpawnQueryFinished(TSharedPtr<FEnvQueryResult> Result);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	UStaticMesh* FoodMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	UMaterialInterface* FoodMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<APickupFood> PickupFoodClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<UPickupUI> PickupUIClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToSpawn = 20.0f;
	float TimePast = 0.0f;
	
	bool SpawnFood(FVector SpawnLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
