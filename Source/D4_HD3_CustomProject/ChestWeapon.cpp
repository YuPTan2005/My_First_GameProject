// Fill out your copyright notice in the Description page of Project Settings.


#include "ChestWeapon.h"


// Sets default values
AChestWeapon::AChestWeapon()
{
	bIsCollectible = false;
}

void AChestWeapon::NotifyCollectible_Implementation()
{
	bIsCollectible = true;
	
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors); 
	PickupCollider->GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor)
		{
			Super::OnOverlap(nullptr, OverlappingActor, nullptr, 0, false, FHitResult());
		}
	}
}

void AChestWeapon::OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsCollectible)
	{
		Super::OnOverlap(OverlapComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

