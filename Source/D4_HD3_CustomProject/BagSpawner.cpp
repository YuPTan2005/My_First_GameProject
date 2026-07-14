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
	if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation RandomNavPoint;
		
		if (NavSys->GetRandomPoint(RandomNavPoint))
		{
			return RandomNavPoint.Location;
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("Failed to find a valid NavMesh for spawning enemy"));
	return FVector::ZeroVector;
}

bool ABagSpawner::SpawnObject()
{
	if (BagClassToSpawn)
	{
		FVector SpawnLocation = GetSpawnPoint();
		FRotator SpawnRotation = FRotator::ZeroRotator;
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		GetWorld()->SpawnActor<AActor>(BagClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
		
		return true;
	}
	
	UE_LOG(LogTemp, Error, TEXT("No value attached to BagClassToSpawn variable in %s. Bag fails to be spawned"), *GetName());
	return false;
}

// Called every frame
void ABagSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

