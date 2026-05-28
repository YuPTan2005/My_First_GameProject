// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Edible.h"
#include "FoodPickupUI.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Food.generated.h"

UCLASS(Abstract)
class D4_HD3_CUSTOMPROJECT_API AFood : public AActor, public IEdible
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFood();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* PickupCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UFoodPickupUI> PickupUIClass;
	
	int ExperienceAmount = 0;
	
	UFoodPickupUI* SpawnedUI;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void EatenBy_Implementation(ACharacter* Character) override;
	virtual void AddPickupUI_Implementation() override;

};
