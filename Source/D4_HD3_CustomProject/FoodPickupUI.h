// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FoodPickupUI.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UFoodPickupUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FVector2D CurrentLocation;
	
	virtual void NativeConstruct() override;
	
};
