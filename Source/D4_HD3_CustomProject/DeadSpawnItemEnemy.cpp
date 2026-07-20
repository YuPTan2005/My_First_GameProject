// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadSpawnItemEnemy.h"

void ADeadSpawnItemEnemy::Dead()
{
	const int RandomIndex = FMath::RandRange(0, ItemClassToSpawn.Num() - 1);
	const TSubclassOf<AActor> RandomItemClass = ItemClassToSpawn[RandomIndex];
	
	GetWorld()->SpawnActor<AActor>(
		RandomItemClass, 
		GetActorLocation(), 
		GetActorRotation()
		);
	
	Super::Dead();
}
