// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FoodPickupUI.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PickupFood.generated.h"

class AFood;

UCLASS()
class D4_HD3_CUSTOMPROJECT_API APickupFood : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupFood();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details")
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* PickupCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UFoodPickupUI> PickupUIClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details")
	AFood* Food;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFoodPickupUI* SpawnedUI;
	
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlapComp, AActor* 
		OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& 
		SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void AddPickupUI(AActor* Actor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Collected();

};
