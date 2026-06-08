// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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
class AD4_HD3_CustomProjectCharacter : public ACharacter
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

public:

	/** Constructor */
	AD4_HD3_CustomProjectCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

protected:
	int UpgradeFactor = 3;
	bool bIsInventoryOpen;
	TArray<APickupFood*> CollectibleFood;
	
	APlayerController* PlayerController;
	
	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;
	
	void Upgrade();
	int CalculateIncreaseAmount(float Attribute);
	
	virtual void Tick(float DeltaSeconds) override;
	
	FTimerHandle DeathTimerHandle;
	int CountDownTime = 10;
	bool bIsCountDownCalled = false;
	void DeathCountDown();
	void Dead();
	void ShowDeathUI();
	FTimerHandle ShowDeathUITimer;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

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
	
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, 
		AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, 
		FVector HitLocation, FVector HitNormal, FVector NormalImpulse, 
		const FHitResult& Hit) override;
	
	int StateNumber = 1;
	const int8 MaxStateNumber = 3;
	void ResetState();
	
	void Eat(IEdible* Food);
	void GainExperience(float ExperienceAmount);
	void GainStarvation(float StarvationAmount);
	
	UInventoryActorComponent* InventoryComponent;
	UInventoryWidget* InventoryWidget;
	
	void ToggleInventory();
	
	AFood* GetItemAtIndex(int32 Index);
	void DeleteItemAtIndex(int32 Index);
	bool AddItem(AFood* NewItem);
	void UseItem(int32 Index);
	
	void AddCollectibleFood(APickupFood* Food);
	void RemoveCollectibleFood(APickupFood* Food);
	
	float Health = 100;
	float MaxHealth = 100;
	float Experience = 0;
	float MaxExperienceLevel = 100;
	float Level = 0;
	
	float MaxStarvationValue = 30;
	float StarvationValue = 30;
	float StarvationDecrement = 0;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerUI> PlayerUIClass;
	UPROPERTY(EditAnywhere)
	UPlayerUI* PlayerUI;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UStarvationUI> StarvationUIClass;
	UPROPERTY(EditAnywhere)
	UStarvationUI* StarvationUI;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DeathUIClass;
	UPROPERTY(EditAnywhere)
	UUserWidget* DeathUI;
	
	void DealDamage(float DamageTook);
	
	bool bCanAttack;
	
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

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

