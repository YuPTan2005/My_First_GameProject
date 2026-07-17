// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem.h"
#include "PickupUI.h"
#include "GameFramework/Actor.h"
#include "PickupFood.generated.h"

UCLASS()
class D4_HD3_CUSTOMPROJECT_API APickupFood : public APickupItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupFood();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPickupUI> EatingUIClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult) override;
	void AddEatingUI();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Collected(AActor* OtherActor);
	virtual void Collected_Implementation(AActor* OtherActor);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnCollected();
	virtual void UnCollected_Implementation();

};
