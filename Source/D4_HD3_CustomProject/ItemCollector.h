// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem.h"
#include "UObject/Interface.h"
#include "ItemCollector.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemCollector : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class D4_HD3_CUSTOMPROJECT_API IItemCollector
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddCollectibleItem(APickupItem* Food);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RemoveCollectibleItem(APickupItem* Food);
	
};
