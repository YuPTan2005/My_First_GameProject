// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "D4_HD3_CustomProjectGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	OnGameStarted();
	if (AD4_HD3_CustomProjectGameMode* Gm = Cast<AD4_HD3_CustomProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		Gm->OnPlayerRespawned.AddDynamic(this, &ASpawner::OnGameStarted);
	}
}

void ASpawner::OnGameStarted()
{
	for (int i=1; i<=GameStartEnemyNumber; i++)
	{
		SpawnObject();
	}
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

