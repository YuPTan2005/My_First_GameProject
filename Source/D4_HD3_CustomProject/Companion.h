// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "Enemy.h"
#include "FoodCollector.h"
#include "GameFramework/Character.h"
#include "Companion.generated.h"

class AD4_HD3_CustomProjectCharacter;

UCLASS()
class D4_HD3_CUSTOMPROJECT_API ACompanion : 
	public ACharacter, 
	public IFoodCollector,
	public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACompanion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Companion")
	float FollowRadius = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Companion")
	float AttackRadius = 1250.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Companion")
	float AttackDistance = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Companion")
	float CollectRadius = 1250.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Companion")
	float CollectDistance = 150.0f;
	
	UPROPERTY()
	TArray<APickupFood*> PickupFoodList;
	UPROPERTY()
	APickupFood* TargetPickupFood;
	
	UPROPERTY()
	AEnemy* TargetEnemy;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlapComp, AActor* 
		OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& 
		SweepResult);
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComponent;
	
	UPROPERTY()
	AD4_HD3_CustomProjectCharacter* CompanionOwner;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 50;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 50;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageValue = 5;
	
	float StarvationValue = 30.0f;
	float MaxStarvationValue = 30.0f;
	float StarvationDecrementValue = -1;
	
	bool bIsDead;
	
	FTimerHandle DeadTimer;
	void Dead();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackInterval = 3.0f;
	float AttackTimer = AttackInterval;
	bool bCanAttack;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CollectInterval = 10.0f;
	float CollectTimer = CollectInterval;
	bool bCanCollect;
	
	UPROPERTY()
	TArray<APickupFood*> CollectibleFoodList;

public:
	float GetCurrentHealth() const;
	float GetMaxHealth() const;
	float GetDamageValue() const;
	float GetStarvationValue() const;
	float GetMaxStarvationValue() const;
	float GetStarvationDecrementValue() const;
	virtual bool IsDead_Implementation() override;
	bool CanAttack() const;
	bool CanCollect() const;
	AD4_HD3_CustomProjectCharacter* GetCompanionOwner();
	bool IsPickupFoodListEmpty();
	float GetFollowRadius() const;
	float GetAttackRadius() const;
	float GetAttackDistance() const;
	float GetCollectRadius() const;
	float GetCollectDistance() const;
	APickupFood* GetTargetPickupFood();
	AEnemy* GetTargetEnemy();
	
	void SetCurrentHealth(float CurrentHealth);
	void SetMaxHealth(float MaxHealth);
	void SetDamageValue(float DamageValue);
	void SetStarvationValue(float StarvationValue);
	void SetMaxStarvationValue(float MaxStarvationValue);
	void SetStarvationDecrementValue(float StarvationDecrementValue);
	void SetIsDead(bool bIsDead);
	void SetCanAttack(bool bCanAttack);
	void SetCanCollect(bool bCanCollect);
	void SetCompanionOwner(AD4_HD3_CustomProjectCharacter* Owner);
	void SetFollowRadius(float NewFollowRadius);
	void SetAttackRadius(float NewAttackRadius);
	void SetAttackDistance(float NewAttackDistance);
	void SetCollectRadius(float NewCollectRadius);
	void SetCollectDistance(float NewCollectDistance);
	void SetTargetPickupFood(APickupFood* NewPickupFood);
	void SetTargetEnemy(AEnemy* Enemy);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void DealDamage_Implementation(float DamageTaken, AActor* DamagedBy) override;
	virtual EGameTeam GetTeam_Implementation() override;
	
	void Attack(AActor* Target);
	
	bool CollectFood();
	
	virtual void AddCollectibleFood_Implementation(APickupFood* Food) override;
	virtual void RemoveCollectibleFood_Implementation(APickupFood* Food) override;
	
	bool IsCollectibleFoodListEmpty();
	
};
