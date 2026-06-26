// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyStatusComponent.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


class UEnemyStatus;

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
	
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageValue = 10;

	void DealDamage(float Damage);
	
	UEnemyStatusComponent* StatusComponent;
	UEnemyStatus* StatusWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> EnemyStatusClass;
	
	void UpdateStatus();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackInterval = 3;

	bool bCanAttack;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* WalkAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* FlyAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* LandingMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* FlyLaunchMontage;
	
	void DisplayFlyLaunchMontage();
	void DisplayLandingMontage();
	
	void Attack(AActor* Target);
	
	bool bIsFlying;
	
	bool bIsDead;
	
	FTimerHandle DeadTimer;
	void Dead();

};
