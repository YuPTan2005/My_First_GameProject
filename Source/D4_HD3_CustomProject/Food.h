// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Edible.h"
#include "InventoryItem.h"
#include "GameFramework/Actor.h"
#include "Food.generated.h"

UCLASS(Abstract)
class D4_HD3_CUSTOMPROJECT_API AFood : public AActor, public IEdible, public IInventoryItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFood();
	
protected:
	float ExperienceAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FoodName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FoodDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StarvationAmount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void EatenBy_Implementation(AActor* Character) override;
	
	void SetName(FString Name);
	void SetDescription(FString Description);
	
	virtual FString GetName_Implementation() override;
	virtual FString GetDescription_Implementation() override;

};
