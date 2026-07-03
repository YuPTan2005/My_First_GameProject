// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Attackable.h"
#include "Companion.h"
#include "D4_HD3_CustomProjectPlayerController.h"
#include "Damageable.h"
#include "DeathUI.h"
#include "FoodCollector.h"
#include "InventoryActorComponent.h"
#include "InventoryWidget.h"
#include "PickupFood.h"
#include "PlayerUI.h"
#include "StarvationUI.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "D4_HD3_CustomProjectCharacter.generated.h"

class AFood;
class IEdible;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AD4_HD3_CustomProjectCharacter : 
	public ACharacter, 
	public IFoodCollector, 
	public IAttackable, 
	public IDamageable
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* CollectAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* InventoryAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* DashAction;

public:

	/** Constructor */
	AD4_HD3_CustomProjectCharacter();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACompanion* Companion;
	
	int UpgradeFactor = 3;
	bool bIsInventoryOpen;
	
	UPROPERTY()
	TArray<APickupFood*> CollectibleFood;
	
	UPROPERTY()
	AD4_HD3_CustomProjectPlayerController* PlayerController;
	
	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;
	
	virtual void Destroyed() override;
	
	virtual void PossessedBy(AController* NewController) override;
	
	void Upgrade();
	int CalculateIncreaseAmount(float Attribute);
	
	virtual void Tick(float DeltaSeconds) override;
	
	bool bIsDead = false;
	
	FTimerHandle DeathTimerHandle;
	int CountDownTime = 10;
	bool bIsCountDownCalled = false;
	void DeathCountDown();
	void Dead();
	void ShowDeathUI();
	FTimerHandle ShowDeathUITimer;

	bool bIsStarvationDecrement = true;
	bool bCanDash = false;
	float DashSpeed = 0.0f;
	
	float DashCoolDown = 1.0f;
	float DashTimer = DashCoolDown;
	
	float OriginalFlyBrake = 1500.0f;
	float DashFlyBrake = 3500.0f;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, 
		AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, 
		FVector HitLocation, FVector HitNormal, FVector NormalImpulse, 
		const FHitResult& Hit) override;

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();
	
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void Collect();
	
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void Dash();
	
	FTimerHandle FlyDashTimerHandle;
	void RestoreFlyBrake();
	
	int StateNumber = 1;
	const int8 MaxStateNumber = 3;
	void ResetState();
	
	void Eat(IEdible* Food);
	void GainExperience(float ExperienceAmount);
	void GainStarvation(float StarvationAmount);
	
	UPROPERTY()
	UInventoryActorComponent* InventoryComponent;
	UPROPERTY()
	UInventoryWidget* InventoryWidget;
	
	void ToggleInventory();
	
	AFood* GetItemAtIndex(int32 Index);
	void DeleteItemAtIndex(int32 Index);
	bool AddItem(AFood* NewItem);
	void UseItem(int32 Index);
	
	virtual void AddCollectibleFood_Implementation(APickupFood* Food) override;
	virtual void RemoveCollectibleFood_Implementation(APickupFood* Food) override;
	
	float Health = 100;
	float MaxHealth = 100;
	float Experience = 0;
	float MaxExperienceLevel = 100;
	float Level = 0;
	
	float MaxStarvationValue = 50;
	float StarvationValue = 50;
	float StarvationDecrementValue = -1;

	UPROPERTY()
	UPlayerUI* PlayerUI;
	UPROPERTY()
	UStarvationUI* StarvationUI;
	UPROPERTY()
	UDeathUI* DeathUI;
	
	virtual void DealDamage_Implementation(float DamageTaken, IAttackable* DamagedBy) override;
	
	bool GetIsDead();
	void SetIsDead(bool NewValue);
	
	bool GetIsStarvationDecrement();
	void SetIsStarvationDecrement(bool NewValue);
	
	bool GetCanDash();
	void SetCanDash(bool NewValue);
	
	float GetDashSpeed();
	void SetDashSpeed(float NewValue);
	
	float GetDashTimer();
	void SetDashTimer(float NewValue);
	
	float GetDashCoolDown();
	void SetDashCoolDown(float NewValue);
	
	UPROPERTY(EditAnywhere)
	float AttackCoolDown = 1.0f;
	UPROPERTY(EditAnywhere)
	float AttackDistance = 200;
	UPROPERTY(EditAnywhere)
	float Damage = 10;
	
	float AttackTimer = AttackCoolDown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* AttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackAnims;
	
	void Attack();
	virtual void Attack_Implementation(IDamageable* Target) override;

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

