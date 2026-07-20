// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadSpawnItemEnemy.h"

void ADeadSpawnItemEnemy::Dead()
{
	GetWorld()->SpawnActor<AActor>(
		ItemClassToSpawn, 
		GetActorLocation(), 
		GetActorRotation()
		);
	
	Super::Dead();
}
