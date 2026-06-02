// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class D4_HD3_CUSTOMPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Ragdoll();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageValue = 100;

	void DealDamage(float Damage);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackInterval = 3;

	bool bCanAttack;
	
	void Attack(AActor* Target);

};
