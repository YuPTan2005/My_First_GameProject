// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Companion.h"
#include "D4_HD3_CustomProjectPlayerController.h"
#include "Damageable.h"
#include "DeathUI.h"
#include "ItemCollector.h"
#include "FoodConsumer.h"
#include "FoodInventoryActorComponent.h"
#include "FoodInventoryWidget.h"
#include "PickupFood.h"
#include "PickupWeapon.h"
#include "PlayerUI.h"
#include "StarvationUI.h"
#include "ViewportInfoUI.h"
#include "Weapon.h"
#include "WeaponInventoryActorComponent.h"
#include "WeaponInventoryWidget.h"
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
	public IItemCollector,
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
	UInputAction* PickupAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* FoodInventoryAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* WeaponInventoryAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* DashAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* EatAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* FeedAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SwapWeaponAction;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Component Class")
	TSubclassOf<UFoodInventoryActorComponent> FoodInventoryComponentClass;
	UPROPERTY()
	UFoodInventoryActorComponent* FoodInventoryComponent;
	
	UPROPERTY()
	TArray<APickupFood*> CollectibleFood;
	UPROPERTY()
	TArray<APickupFood*> EdibleFood;
	UPROPERTY()
	TArray<APickupWeapon*> CollectibleWeapon;
	
	void Collect();
	void Pickup();
	void Eat();
	void Feed();
	void Eat(AActor* Consumer, const FString& EatenText);
	
	void AddInfoUIToViewport(
		const FVector& ItemLocation,
		const bool CollectionResult,
		const FString& SuccessCollectionText, 
		const FLinearColor SuccessCollectionColor,
		TOptional<FString> FailCollectionText = TOptional<FString>(),
		TOptional<FLinearColor> FailCollectionColor = TOptional<FLinearColor>()
		) const;
	
	bool bIsWeaponInventoryOpen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Component Class")
	TSubclassOf<UWeaponInventoryActorComponent> WeaponInventoryComponentClass;
	UPROPERTY()
	UWeaponInventoryActorComponent* WeaponInventoryComponent;
	
	int8 WeaponUsingIndex;
	void SwapWeapon();
	
	UPROPERTY()
	AD4_HD3_CustomProjectPlayerController* PlayerController;
	
	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void PossessedBy(AController* NewController) override;
	
	void InitialiseCompanionUI();
	
	void Upgrade();
	float CalculateIncreaseAmount(float Attribute) const;
	
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
	
	FString FoodSuccessCollectedText = "Food added to inventory!";
	FString FoodFailCollectedText = "Food inventory is full!";
	FString WeaponSuccessCollectedText = "Weapon added to inventory!";
	FString WeaponFailCollectedText = "Weapon inventory is full!";
	FString FoodEatenText = "Food eaten!";
	FString FoodFedText = "Food fed!";
	
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
	
	UPROPERTY()
	UFoodInventoryWidget* FoodInventoryWidget;
	void ToggleInventory();
	
	AActor* GetItemAtIndex(const int8 Index) const;
	void DeleteItemAtIndex(const int8 Index) const;
	bool AddItem(AFood* NewItem) const;
	void UseItem(const int8 Index);
	void FeedItem(const int8 Index) const;
	
	UPROPERTY()
	UWeaponInventoryWidget* WeaponInventoryWidget;
	void ToggleWeaponInventory();
	
	AActor* GetWeaponAtIndex(const int8 Index) const;
	void DeleteWeaponAtIndex(const int8 Index) const;
	bool AddWeapon(AActor* NewItem) const;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor WeaponOnUsedColor = FLinearColor(0.0f, 0.45f, 0.95f);
	void UseWeapon(const int8 Index);
	void UnuseWeapon(const int8 WeaponIndex);
	void DisattachWeaponFromSocket(AWeapon* Weapon) const;
	void AttachWeaponToSocket(AWeapon* Weapon, const FString& SocketName) const;
	
	virtual void AddCollectibleItem_Implementation(APickupItem* Item) override;
	virtual void RemoveCollectibleItem_Implementation(APickupItem* Item) override;
	
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
	
	virtual float GetCurrentHealth_Implementation() const override;
	virtual void SetCurrentHealth_Implementation(const float NewValue) override;
	
	virtual float GetMaxHealth_Implementation() const override;
	virtual void SetMaxHealth_Implementation(const float NewValue) override;
	
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
	
	bool GetIsFoodInventoryFull() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCoolDown = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDistance = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 10;
	float UpgradeDamageValue;
	void IncreaseDamageValue(const float DamageValue);
	
	float AttackTimer = AttackCoolDown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* AttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	TArray<UAnimMontage*> DefaultAttackAnims;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	TArray<UAnimMontage*> SwordAttackAnims;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	TArray<UAnimMontage*> HammerAttackAnims;
	
	void Attack();
	
	void ShowCompanionStarvationUI() const;
	void ClearCompanionStarvationUI() const;
	void OnCompanionDie() const;

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

