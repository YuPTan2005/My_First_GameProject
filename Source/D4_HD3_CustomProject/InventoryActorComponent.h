// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryActorComponent.generated.h"


class AD4_HD3_CustomProjectCharacter;
class AFood;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class D4_HD3_CUSTOMPROJECT_API UInventoryActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryActorComponent();
	
	TArray<AActor*> GetAllItems();
	AActor* GetItemAtIndex(int32 Index);
	virtual bool UseItemAtIndex(int32 Index, AActor* Character);
	bool DeleteItemAtIndex(int32 Index);
	bool AddItem(AActor* NewItem);
	UFUNCTION(BlueprintPure)
	bool IsFull() const;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	int32 InventorySize;
	
	UPROPERTY()
	TArray<AActor*> InventoryItems;
	
};
