// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawningObject.h"

// Sets default values
ASpawningObject::ASpawningObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpawnSphereArea = CreateDefaultSubobject<USphereComponent>(TEXT("Spawn Area Component"));
	RootComponent = SpawnSphereArea;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	MeshComponent->SetupAttachment(RootComponent);
	
	SpawnSphereArea->SetSphereRadius(500.0f);
}

// Called when the game starts or when spawned
void ASpawningObject::BeginPlay()
{
	Super::BeginPlay();
	
	if (!SpawnFood())
	{
		UE_LOG(LogTemp, Error, TEXT("Food fails to be spawned"));
	}
}

// Called every frame
void ASpawningObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TimePast += DeltaTime;
	if (TimePast >= TimeToSpawn)
	{
		TimePast = 0.0f;
		if (!SpawnFood())
		{
			UE_LOG(LogTemp, Error, TEXT("Food fails to be spawned"));
		}
	}
}

FVector ASpawningObject::GetRandomSpawnPoint()
{
	if (SpawnSphereArea)
	{
		FVector SphereCenter = SpawnSphereArea->GetComponentLocation();
		float Radius = SpawnSphereArea->GetUnscaledSphereRadius();
		
		FVector RandomDirection = FMath::VRand();
		RandomDirection.Z = 0.0f; 
		RandomDirection.Normalize();
		float RandomRadius = FMath::FRandRange(0.0f, Radius);

		return SphereCenter + RandomDirection * RandomRadius;
	}
	
	UE_LOG(LogTemp, Error, TEXT("No SphereComponent created for SpawningTree"));
	return FVector::ZeroVector;
}

bool ASpawningObject::SpawnFood()
{
	if (PickupFoodClass && FoodToSpawn)
	{
		FVector SpawnLocation = GetRandomSpawnPoint();
		FRotator SpawnRotation = FRotator::ZeroRotator;
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		APickupFood* PickupFoodSpawned = GetWorld()->
			SpawnActor<APickupFood>(PickupFoodClass, SpawnLocation, SpawnRotation, SpawnParams);
		AFood* FoodSpawned = GetWorld()->
			SpawnActor<AFood>(FoodToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
		
		PickupFoodSpawned->Food = FoodSpawned;
		if (FoodMesh && FoodMaterial && PickupUIClass)
		{
			PickupFoodSpawned->MeshComponent->SetStaticMesh(FoodMesh);
			PickupFoodSpawned->MeshComponent->SetMaterial(0, FoodMaterial);
			PickupFoodSpawned->PickupUIClass = PickupUIClass;
		}
		else
		{
			UE_LOG(LogTemp, Warning, 
				TEXT("No FoodMesh, FoodMaterial, and PickupUIClass attached to SpawningTree class"));
		}
		
		return true;
	}
	
	return false;
}

