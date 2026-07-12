// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Companion.h"
#include "D4_HD3_CustomProjectPlayerController.h"
#include "Damageable.h"
#include "DeathUI.h"
#include "FoodCollector.h"
#include "FoodConsumer.h"
#include "InventoryActorComponent.h"
#include "InventoryWidget.h"
#include "PickupFood.h"
#include "PlayerUI.h"
#include "StarvationUI.h"
#include "ViewportInfoUI.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "D4_HD3_CustomProjectCharacter.generated.h"

class UCompanionStarvationUI;
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
	public IDamageable,
	public IFoodConsumer
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
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* EatAction;

public:

	/** Constructor */
	AD4_HD3_CustomProjectCharacter();

protected:
	float Health = 100;
	float MaxHealth = 100;
	float Experience = 0;
	float MaxExperience = 100;
	float Level = 1;
	float MaxLevel = 30;
	
	int UpgradeFactor = 3;
	
	float MaxStarvationValue = 50;
	float StarvationValue = 50;
	float StarvationDecrementValue = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACompanion> CompanionClass;
	UPROPERTY()
	ACompanion* Companion;
	
	bool bIsInventoryOpen;
	
	UPROPERTY()
	TArray<APickupFood*> CollectibleFood;
	UPROPERTY()
	TArray<APickupFood*> EdibleFood;
	
	UPROPERTY()
	AD4_HD3_CustomProjectPlayerController* PlayerController;
	
	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void PossessedBy(AController* NewController) override;
	
	void Upgrade();
	int CalculateIncreaseAmount(float Attribute) const;
	
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
	
	void Collect();
	void Eat();
	
	FString FoodSuccessCollectedText = "Food added to inventory!";
	FString FoodFailCollectedText = "Inventory is full!";
	FString FoodEatenText = "Food eaten!";
	
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void Dash();
	
	FTimerHandle FlyDashTimerHandle;
	void RestoreFlyBrake();
	
	int StateNumber = 1;
	const int8 MaxStateNumber = 3;
	void ResetState();
	
	virtual void Eat_Implementation(AActor* Food) override;
	void GainExperience(float ExperienceAmount);
	virtual void GainStarvation_Implementation(float StarvationAmount) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Component Class")
	TSubclassOf<UInventoryActorComponent> InventoryComponentClass;
	
	UPROPERTY()
	UInventoryActorComponent* InventoryComponent;
	UPROPERTY()
	UInventoryWidget* InventoryWidget;
	
	void ToggleInventory();
	
	AFood* GetItemAtIndex(int32 Index);
	void DeleteItemAtIndex(int32 Index);
	bool AddItem(AFood* NewItem);
	void UseItem(int32 Index);
	void FeedItem(int32 Index) const;
	
	virtual void AddCollectibleFood_Implementation(APickupFood* Food) override;
	virtual void RemoveCollectibleFood_Implementation(APickupFood* Food) override;
	
	UPROPERTY()
	UPlayerUI* PlayerUI;
	UPROPERTY()
	UStarvationUI* StarvationUI;
	UPROPERTY()
	UCompanionStarvationUI* CompanionStarvationUI;
	UPROPERTY()
	UDeathUI* DeathUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UViewportInfoUI> ViewportInfoUIClass;
	
	virtual void DealDamage_Implementation(float DamageTaken, AActor* DamagedBy) override;
	virtual EGameTeam GetTeam_Implementation() override;
	
	virtual bool IsDead_Implementation() override;
	void SetIsDead(const bool NewValue);
	
	float GetCurrentLevel() const;
	void SetCurrentLevel(const float NewValue);
	
	float GetExperience() const;
	void SetExperience(const float NewValue);
	
	float GetMaxExperience() const;
	void SetMaxExperience(const float NewValue);
	
	float GetCurrentHealth() const;
	void SetCurrentHealth(const float NewValue);
	
	float GetMaxHealth() const;
	void SetMaxHealth(const float NewValue);
	
	float GetStarvationValue() const;
	void SetStarvationValue(const float NewValue);
	
	float GetMaxStarvationValue() const;
	void SetMaxStarvationValue(const float NewValue);
	
	bool GetIsStarvationDecrement() const;
	void SetIsStarvationDecrement(const bool NewValue);
	
	bool GetCanDash() const;
	void SetCanDash(const bool NewValue);
	
	float GetDashSpeed() const;
	void SetDashSpeed(const float NewValue);
	
	float GetDashTimer() const;
	void SetDashTimer(const float NewValue);
	
	float GetDashCoolDown() const;
	void SetDashCoolDown(const float NewValue);
	
	bool GetHasBackpack() const;
	void SetHasBackpack(const bool NewValue) const;
	
	ACompanion* GetCompanion() const;
	void SetCompanion(ACompanion* NewCompanion);
	
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
	
	void ShowCompanionStarvationUI() const;
	void ClearCompanionStarvationUI() const;

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

