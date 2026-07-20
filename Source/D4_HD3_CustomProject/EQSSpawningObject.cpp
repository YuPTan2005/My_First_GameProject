// Fill out your copyright notice in the Description page of Project Settings.


#include "EQSSpawningObject.h"

#include "EnvironmentQuery/EnvQueryManager.h"

// Sets default values
AEQSSpawningObject::AEQSSpawningObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

void AEQSSpawningObject::TriggerSpawnLocationQuery()
{
	if (!SpawnQuery)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnQuery asset is missing on %s"), *GetName());
		return;
	}

	// Create the asynchronous request setup
	FEnvQueryRequest QueryRequest(SpawnQuery, this);
    
	// Execute the query
	QueryRequest.Execute(
		EEnvQueryRunMode::SingleResult,
		this, 
		&AEQSSpawningObject::OnSpawnQueryFinished
	);
}

void AEQSSpawningObject::OnSpawnQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (Result.IsValid() && Result->IsSuccessful() && Result->Items.Num() > 0)
	{
		SpawnFood(Result->GetItemAsLocation(0));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EQS failed to find any valid spawn positions around the tree."));
	}
}

// Called when the game starts or when spawned
void AEQSSpawningObject::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerSpawnLocationQuery();
}

bool AEQSSpawningObject::SpawnFood(FVector SpawnLocation)
{
	if (PickupFoodClass)
	{
		FRotator SpawnRotation = FRotator::ZeroRotator;
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		if (FoodMesh && FoodMaterial && PickupUIClass)
		{
			APickupFood* PickupFoodSpawned = GetWorld()->
			SpawnActor<APickupFood>(PickupFoodClass, SpawnLocation, SpawnRotation, SpawnParams);
			
			PickupFoodSpawned->MeshComponent->SetStaticMesh(FoodMesh);
			PickupFoodSpawned->MeshComponent->SetMaterial(0, FoodMaterial);
			PickupFoodSpawned->PickupUIClass = PickupUIClass;
		}
		else
		{
			UE_LOG(LogTemp, Warning, 
				TEXT("No FoodMesh, FoodMaterial, and PickupUIClass attached to %s"), *GetName());
		}
		
		return true;
	}
	
	return false;
}

// Called every frame
void AEQSSpawningObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimePast += DeltaTime;
	if (TimePast >= TimeToSpawn)
	{
		TimePast = 0.0f;
		TriggerSpawnLocationQuery();
	}
}

