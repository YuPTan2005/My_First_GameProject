// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodPickupUI.h"

void UFoodPickupUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetPositionInViewport(CurrentLocation);
}
