// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDropActorComponent.h"

// Sets default values for this component's properties
UItemDropActorComponent::UItemDropActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UItemDropActorComponent::SpawnRandomItem()
{
	if (ItemClassesToSpawn.Num() == 0)
	{
		return;
	}

	const AActor* Owner = GetOwner();
	UWorld* World = GetWorld();

	if (!Owner || !World)
	{
		return;
	}
	
	const int8 RandomIndex = FMath::RandRange(0, ItemClassesToSpawn.Num() - 1);
	
	if (const TSubclassOf<AActor> RandomItemClass = ItemClassesToSpawn[RandomIndex])
	{
		World->SpawnActor<AActor>(
			RandomItemClass,
			Owner->GetActorLocation(),
			Owner->GetActorRotation()
		);
	}
}

