// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DestructibleItem.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PickupItem.h"
#include "DestructibleChest.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API ADestructibleChest : public ADestructibleItem
{
	GENERATED_BODY()
	
public:
	ADestructibleChest();
	
protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Reset() override;
	
	FTransform ChestTopInitial;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mesh")
	UMeshComponent* ChestTopMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mesh")
	UMeshComponent* ChestBottomMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mesh")
	UPhysicsConstraintComponent* ChestHinge;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<APickupItem>> ChestItemsClasses;
	UPROPERTY()
	TArray<APickupItem*> ChestItems;
	
	virtual void SetupChestItems();
	
	virtual void OnDestructed() override;
	
};
