// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupFood.h"
#include "UObject/Interface.h"
#include "FoodCollector.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFoodCollector : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class D4_HD3_CUSTOMPROJECT_API IFoodCollector
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddCollectibleFood(APickupFood* Food);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RemoveCollectibleFood(APickupFood* Food);
	
};
