// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryActorComponent.h"
#include "WeaponInventoryActorComponent.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UWeaponInventoryActorComponent : public UInventoryActorComponent
{
	GENERATED_BODY()
	
public:
	UWeaponInventoryActorComponent();
	
	virtual bool UseItemAtIndex(int32 Index, AActor* Character) override;
	
};
