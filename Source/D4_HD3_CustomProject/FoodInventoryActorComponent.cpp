// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodInventoryActorComponent.h"
#include "FoodConsumer.h"


bool UFoodInventoryActorComponent::UseItemAtIndex(int32 Index, AActor* Character)
{
	if(Index >= InventoryItems.Num() || Index < 0)
	{
		return false;
	}
	
	if (Character->Implements<UFoodConsumer>())
	{
		AActor* UsedItem = InventoryItems[Index];
		IFoodConsumer::Execute_Eat(Character, UsedItem);
		InventoryItems.RemoveAt(Index);
		
		return true;
	}
	
	return false;
}

bool UFoodInventoryActorComponent::GetHasBackpack() const
{
	return bHasBackpack;
}

void UFoodInventoryActorComponent::SetHasBackpack(const bool NewValue)
{
	bHasBackpack = NewValue;
}
