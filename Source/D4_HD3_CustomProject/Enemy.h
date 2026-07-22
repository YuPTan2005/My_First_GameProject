// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "NPCInterface.h"
#include "NPCStatusComponent.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


class UNPCStatus;

UCLASS(Abstract)
class D4_HD3_CUSTOMPROJECT_API AEnemy : public ACharacter, public IDamageable, public INPCInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Reset() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FVector StatusComponentOffset = FVector(0.0f, 0.0f, 0.0f);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageValue = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefeatedDamageValue = 5;
	
	virtual float GetCurrentHealth_Implementation() override;
	virtual float GetMaxHealth_Implementation() override;

	virtual void DealDamage_Implementation(float DamageTaken, AActor* DamagedBy) override;
	virtual bool IsDead_Implementation() override;
	virtual EGameTeam GetTeam_Implementation() override;
	
	UPROPERTY()
	UNPCStatusComponent* StatusComponent;
	UPROPERTY()
	UNPCStatus* StatusWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> EnemyStatusClass;
	
	void SetCurrentHealth(const float NewValue);
	void SetMaxHealth(const float NewValue);
	void UpdateStatus();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackInterval = 3;
	bool bCanAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDistance = 200.0f;
	
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
	virtual void Dead();

};
