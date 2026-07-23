// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GameStartEnemyNumber = 3;
	bCanSpawnRestrictedEnemy = false;
	EnemyExtraHealth = 0.0f;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(
		RestrictedEnemyTimeTracker,
		this,
		&AEnemySpawner::ToggleCanSpawnRestrictedEnemy,
		TimeSpawnRestrictedEnemy,
		false
		);
}

void AEnemySpawner::OnGameStarted()
{
	EnemyExtraHealth = 0.0f;
	TimePast = 0.0f;
	
	Super::OnGameStarted();
	
	EnemyExtraHealth = SpawnHealthIncreaseValue;
}

FVector AEnemySpawner::GetSpawnPoint()
{
	if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavAgentProperties AgentProps;
		AgentProps.AgentRadius = 50.0f;
		AgentProps.AgentHeight = 250.0f; 
		
		ANavigationData* TargetNavData = NavSys->GetNavDataForProps(AgentProps);
		
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		const FVector PlayerLocation = PlayerPawn ? PlayerPawn->GetActorLocation() : FVector::ZeroVector;
		
		for (int32 Attempt = 0; Attempt < 15; ++Attempt)
		{
			FNavLocation RandomNavPoint;
			if (NavSys->GetRandomPoint(RandomNavPoint, TargetNavData))
			{
				if (!PlayerPawn || FVector::DistSquared(RandomNavPoint.Location, PlayerLocation) >= FMath::Square(2500.0f))
				{
					return RandomNavPoint.Location;
				}
			}
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("Failed to find a valid NavMesh for spawning enemy"));
	return FVector::ZeroVector;
}

bool AEnemySpawner::SpawnDefaultActor()
{
	if (!AlwaysSpawnEnemyClass.IsEmpty())
	{
		const int RandomEnemyIndex = FMath::RandRange(0, AlwaysSpawnEnemyClass.Num()-1);
		TSubclassOf<AEnemy> SpawnEnemyClass;
		
		if (!DropHealPotionEnemyClass.IsEmpty() && FMath::RandRange(0.0f, 1.0f) <= HealPotionEnemyPercent)
		{
			SpawnEnemyClass = DropHealPotionEnemyClass[RandomEnemyIndex];
		}
		else
		{
			SpawnEnemyClass = AlwaysSpawnEnemyClass[RandomEnemyIndex];
		}
		
		if (IsValid(SpawnObject(SpawnEnemyClass)))
		{
			return true;
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("Enemy fails to be spawned"));
	return false;
}

bool AEnemySpawner::SpawnRestrictedEnemy()
{
	if (!RestrictedSpawnEnemyClass.IsEmpty())
	{
		int RandomEnemyIndex = FMath::RandRange(0, RestrictedSpawnEnemyClass.Num()-1);
		TSubclassOf<AEnemy> SpawnEnemyClass = RestrictedSpawnEnemyClass[RandomEnemyIndex];
		
		if (IsValid(SpawnObject(SpawnEnemyClass)))
		{
			return true;
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("Enemy fails to be spawned"));
	return false;
}

void AEnemySpawner::ToggleCanSpawnRestrictedEnemy()
{
	bCanSpawnRestrictedEnemy = true;
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimePast += DeltaTime;
	if (TimePast >= TimeToSpawn)
	{
		if (bCanSpawnRestrictedEnemy && 
			NumberOfRestrictedEnemy > 0 &&
			FMath::RandRange(0.0f, 1.0f) < NumberOfRestrictedEnemy
			)
		{
			SpawnRestrictedEnemy();
			NumberOfRestrictedEnemy -= 1;
		}
		else
		{
			SpawnDefaultActor();
		}
		
		EnemyExtraHealth += SpawnHealthIncreaseValue;
		TimePast = 0.0f;
	}
}

