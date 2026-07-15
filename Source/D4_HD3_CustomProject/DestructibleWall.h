// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "NPCStatusComponent.h"
#include "GameFramework/Actor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "DestructibleWall.generated.h"

UCLASS()
class D4_HD3_CUSTOMPROJECT_API ADestructibleWall : public AActor, public IDamageable
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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UNPCStatusComponent> HealthWidgetComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Destruction")
	AFieldSystemActor* MasterField;
	
	UPROPERTY(EditAnywhere, Category="Health")
	float MaxHealth = 1500.0f;
	float CurrentHealth;
	
	bool bIsDead;
	
	void UpdateStatus() const;
	void TriggerExplosion() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void DealDamage_Implementation(float DamageTaken, AActor* DamagedBy) override;
	virtual bool IsDead_Implementation() override;
	virtual EGameTeam GetTeam_Implementation() override;

};
