// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChestItem.h"
#include "PickupWeapon.h"
#include "ChestHammer.generated.h"

UCLASS()
class D4_HD3_CUSTOMPROJECT_API AChestHammer : public APickupWeapon, public IChestItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChestHammer();
	virtual void NotifyCollectible_Implementation() override;

protected:
	bool bIsCollectible;
	
	virtual void OnOverlap(UPrimitiveComponent* OverlapComp, AActor* 
		OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& 
		SweepResult) override;
	
};
