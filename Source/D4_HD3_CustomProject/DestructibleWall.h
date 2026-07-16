// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DestructibleItem.h"
#include "GameFramework/Actor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "DestructibleWall.generated.h"

UCLASS()
class D4_HD3_CUSTOMPROJECT_API ADestructibleWall : public ADestructibleItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructibleWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Destruction")
	TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Destruction")
	AFieldSystemActor* MasterField;
	
	virtual void OnDestructed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void DealDamage_Implementation(float DamageTaken, AActor* DamagedBy) override;

};
