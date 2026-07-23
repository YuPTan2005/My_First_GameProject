// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "NavigationSystem.h"

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
	if (!AlwaysSpawnEnemyClass.IsEmpty())
	{
		const int RandomEnemyIndex = FMath::RandRange(0, AlwaysSpawnEnemyClass.Num()-1);
		TSubclassOf<AEnemy> AlwaysSpawnEnemyClassToSpawn;
		
		if (!DropHealPotionEnemyClass.IsEmpty() && FMath::RandRange(0.0f, 1.0f) <= HealPotionEnemyPercent)
		{
			AlwaysSpawnEnemyClassToSpawn = DropHealPotionEnemyClass[RandomEnemyIndex];
		}
		else
		{
			AlwaysSpawnEnemyClassToSpawn = AlwaysSpawnEnemyClass[RandomEnemyIndex];
		}
		
		const FVector SpawnLocation = GetSpawnPoint();
		const FRotator SpawnRotation = FRotator::ZeroRotator;
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(AlwaysSpawnEnemyClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
		const float EnemyNewHealth = NewEnemy->GetMaxHealth_Implementation() + EnemyExtraHealth;
		NewEnemy->SetMaxHealth(EnemyNewHealth);
		NewEnemy->SetCurrentHealth(EnemyNewHealth);
		NewEnemy->UpdateStatus();
		
		return true;
	}
	
	UE_LOG(LogTemp, Error, TEXT("Enemy fails to be spawned"));
	return false;
}

bool AEnemySpawner::SpawnRestrictedEnemy()
{
	if (!RestrictedSpawnEnemyClass.IsEmpty())
	{
		FVector SpawnLocation = GetSpawnPoint();
		FRotator SpawnRotation = FRotator::ZeroRotator;
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		int RandomEnemyIndex = FMath::RandRange(0, RestrictedSpawnEnemyClass.Num()-1);
		TSubclassOf<AEnemy> EnemyClassToSpawn = RestrictedSpawnEnemyClass[RandomEnemyIndex];
		
		AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
		const float EnemyNewHealth = NewEnemy->GetMaxHealth_Implementation() + EnemyExtraHealth;
		NewEnemy->SetMaxHealth(EnemyNewHealth);
		NewEnemy->SetCurrentHealth(EnemyNewHealth);
		NewEnemy->UpdateStatus();
		
		return true;
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
			SpawnObject();
		}
		
		EnemyExtraHealth += SpawnHealthIncreaseValue;
		TimePast = 0.0f;
	}
}

