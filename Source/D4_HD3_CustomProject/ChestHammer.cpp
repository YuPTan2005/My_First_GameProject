// Fill out your copyright notice in the Description page of Project Settings.


#include "ChestHammer.h"


// Sets default values
AChestHammer::AChestHammer()
{
	bIsCollectible = false;
}

void AChestHammer::NotifyCollectible_Implementation()
{
	bIsCollectible = true;
}

void AChestHammer::OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsCollectible)
	{
		Super::OnOverlap(OverlapComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

