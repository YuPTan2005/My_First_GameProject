// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemDropActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class D4_HD3_CUSTOMPROJECT_API UItemDropActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemDropActorComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Loot")
	void SpawnRandomItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TArray<TSubclassOf<AActor>> ItemClassesToSpawn;
		
};
