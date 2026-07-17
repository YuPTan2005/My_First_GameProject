// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupUI.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

UCLASS()
class D4_HD3_CUSTOMPROJECT_API APickupItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPickupUI> PickupUIClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool bIsNotPickedUp = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* PickupCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details")
	AActor* Item;
	
	virtual void Reset() override;
	
	UPROPERTY()
	UPickupUI* SpawnedUI;
	
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlapComp, AActor* 
		OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& 
		SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void AddPickupUI();
	
	UPROPERTY()
	AActor* PickerActor;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	bool GetIsNotPickedUp() const;
	void SetIsNotPickedUp(bool NewValue);
	AActor* GetItem() const;
	void SetItem(AActor* NewItem);
	
	UFUNCTION(BlueprintPure)
	AActor* GetPickerActor() const;
	UFUNCTION(BlueprintCallable)
	void SetPickerActor(AActor* NewActor);
	
	void PickedUp();

};
