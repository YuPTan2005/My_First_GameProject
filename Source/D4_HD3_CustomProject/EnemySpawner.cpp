// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "NavigationSystem.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GameStartEnemyNumber = 3;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

}

FVector AEnemySpawner::GetSpawnPoint()
{
	if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavAgentProperties AgentProps;
		AgentProps.AgentRadius = 50.0f;
		AgentProps.AgentHeight = 160.0f; 
		
		ANavigationData* TargetNavData = NavSys->GetNavDataForProps(AgentProps);
		
		FNavLocation RandomNavPoint;
		
		if (NavSys->GetRandomPoint(RandomNavPoint, TargetNavData))
		{
			return RandomNavPoint.Location;
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("Failed to find a valid NavMesh for spawning enemy"));
	return FVector::ZeroVector;
}

bool AEnemySpawner::SpawnObject()
{
	if (!EnemyClass.IsEmpty())
	{
		FVector SpawnLocation = GetSpawnPoint();
		FRotator SpawnRotation = FRotator::ZeroRotator;
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		int RandomEnemyIndex = FMath::RandRange(0, EnemyClass.Num()-1);
		TSubclassOf<AEnemy> EnemyClassToSpawn = EnemyClass[RandomEnemyIndex];
		
		GetWorld()->SpawnActor<AEnemy>(EnemyClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
		
		return true;
	}
	
	UE_LOG(LogTemp, Error, TEXT("Enemy fails to be spawned"));
	return false;
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimePast += DeltaTime;
	if (TimePast >= TimeToSpawn)
	{
		TimePast = 0.0f;
		SpawnObject();
	}
}

