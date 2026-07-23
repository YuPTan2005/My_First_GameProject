// Fill out your copyright notice in the Description page of Project Settings.


#include "BagSpawner.h"

#include "NavigationSystem.h"

// Sets default values
ABagSpawner::ABagSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	GameStartEnemyNumber = 1;

}

// Called when the game starts or when spawned
void ABagSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ABagSpawner::GetSpawnPoint()
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys)
	{
		UE_LOG(LogTemp, Error, TEXT("Navigation System not found in %s!"), *GetName());
		return FVector::ZeroVector;
	}

	FNavLocation RandomNavPoint;

	constexpr int32 MaxAttempts = 15; 
	constexpr float BackpackRadius = 40.0f; 
	const FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(BackpackRadius);

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	for (int32 Attempt = 0; Attempt < MaxAttempts; ++Attempt)
	{
		if (NavSys->GetRandomPoint(RandomNavPoint))
		{
			FVector TestLocation = RandomNavPoint.Location + FVector(0, 0, 15.0f) +
									FVector(0.0f, 0.0f, BackpackRadius);

			bool bOverlapsStaticMesh = GetWorld()->OverlapAnyTestByChannel(
				TestLocation,
				FQuat::Identity,
				ECC_WorldStatic,
				CollisionSphere,
				TraceParams
			);

			if (!bOverlapsStaticMesh)
			{
				return RandomNavPoint.Location - FVector(0.0f, 0.0f, BackpackRadius); 
			}
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("Failed to find a valid NavMesh for spawning enemy"));
	return FVector::ZeroVector;
}

bool ABagSpawner::SpawnDefaultActor()
{
	if (BagClassToSpawn)
	{
		if (IsValid(Super::SpawnObject(BagClassToSpawn)))
		{
			return true;
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("No value attached to BagClassToSpawn variable in %s. Bag fails to be spawned"), *GetName());
	return false;
}

// Called every frame
void ABagSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

