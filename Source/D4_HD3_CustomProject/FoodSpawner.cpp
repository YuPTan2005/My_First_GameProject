// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodSpawner.h"

// Sets default values
AFoodSpawner::AFoodSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GameStartEnemyNumber = 1;
	
	SpawnSphereArea = CreateDefaultSubobject<USphereComponent>(TEXT("Spawn Area Component"));
	RootComponent = SpawnSphereArea;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	MeshComponent->SetupAttachment(RootComponent);
	
	SpawnSphereArea->SetSphereRadius(500.0f);
}

// Called when the game starts or when spawned
void AFoodSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFoodSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TimePast += DeltaTime;
	if (TimePast >= TimeToSpawn)
	{
		TimePast = 0.0f;
		SpawnObject();
	}
}

FVector AFoodSpawner::GetSpawnPoint()
{
	if (SpawnSphereArea)
	{
		FVector SphereCenter = SpawnSphereArea->GetComponentLocation();
		float Radius = SpawnSphereArea->GetScaledSphereRadius();
		float MeshRadius = 0.0f;
		// Avoid too near to the center of the mesh
		if (UStaticMesh* StaticMesh = MeshComponent->GetStaticMesh())
		{
			MeshRadius = StaticMesh->GetBounds().SphereRadius;
		}
		
		constexpr int8 MaxAttempts = 15; 
		constexpr float FoodRadius = 30.0f; 
		const FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(FoodRadius);

		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		
		for (int32 Attempt = 0; Attempt < MaxAttempts; ++Attempt)
		{
			FVector RandomDirection = FMath::VRand();
			RandomDirection.Z = 0.0f; 
			RandomDirection.Normalize();
			float RandomRadius = FMath::FRandRange(0.0f, Radius);
			
			FVector RandomLocation = SphereCenter + (RandomDirection * (MeshRadius + RandomRadius)) + 
									 FVector(0, 0, 15.0f) + FVector(0.0f, 0.0f, FoodRadius);

			bool bOverlapsStaticMesh = GetWorld()->OverlapAnyTestByChannel(
				RandomLocation,
				FQuat::Identity,
				ECC_WorldStatic,
				CollisionSphere,
				TraceParams
			);

			if (!bOverlapsStaticMesh)
			{
				return RandomLocation; 
			}
		}
	
		UE_LOG(LogTemp, Error, TEXT("Failed to find a valid location for spawning food item"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No SphereComponent created for FoodSpawner"));
	}
	
	return FVector::ZeroVector;
}

bool AFoodSpawner::SpawnObject()
{
	if (PickupFoodClass)
	{
		FVector SpawnLocation = GetSpawnPoint();
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
	
	UE_LOG(LogTemp, Error, TEXT("Food fails to be spawned"));
	return false;
}

