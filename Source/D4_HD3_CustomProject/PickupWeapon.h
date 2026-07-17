// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem.h"
#include "PickupWeapon.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API APickupWeapon : public APickupItem
{
	GENERATED_BODY()
	
public:
	APickupWeapon();
	virtual void PickedUp() override;
	
};
