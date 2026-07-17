// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryActorComponent.h"
#include "FoodInventoryActorComponent.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UFoodInventoryActorComponent : public UInventoryActorComponent
{
	GENERATED_BODY()
	
public:
	virtual bool UseItemAtIndex(int32 Index, AActor* Character) override;
	
	UFUNCTION(BlueprintPure)
	bool GetHasBackpack() const;
	UFUNCTION(BlueprintCallable)
	void SetHasBackpack(const bool NewValue);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter = GetHasBackpack,Setter = SetHasBackpack)
	bool bHasBackpack;
	
};
