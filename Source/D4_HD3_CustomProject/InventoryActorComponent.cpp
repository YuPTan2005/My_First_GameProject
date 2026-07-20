// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryActorComponent.h"


// Sets default values for this component's properties
UInventoryActorComponent::UInventoryActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

TArray<AActor*> UInventoryActorComponent::GetAllItems()
{
	return InventoryItems;
}

AActor* UInventoryActorComponent::GetItemAtIndex(int32 Index)
{
	if(Index >= InventoryItems.Num() || Index < 0)
	{
		return nullptr;
	}
	return InventoryItems[Index];
}

bool UInventoryActorComponent::UseItemAtIndex(int32 Index, AActor* Character)
{
	if(Index >= InventoryItems.Num() || Index < 0)
	{
		return false;
	}
	InventoryItems.RemoveAt(Index);
	return true;
}

bool UInventoryActorComponent::DeleteItemAtIndex(int32 Index)
{
	if(Index >= InventoryItems.Num() || Index < 0) {
		return false;
	}
	InventoryItems.RemoveAt(Index);
	return true;
}

bool UInventoryActorComponent::AddItem(AActor* NewItem)
{
	if(IsFull()) return false;
	InventoryItems.Add(NewItem);
	return true;
}

bool UInventoryActorComponent::IsFull() const
{
	return InventoryItems.Num() >= InventorySize;
}

int UInventoryActorComponent::GetInventorySize() const
{
	return InventorySize;
}

// Called when the game starts
void UInventoryActorComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
