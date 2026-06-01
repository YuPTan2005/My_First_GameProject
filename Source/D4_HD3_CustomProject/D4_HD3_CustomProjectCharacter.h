// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryActorComponent.h"
#include "InventoryWidget.h"
#include "PickupFood.h"
#include "PlayerUI.h"
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
	
	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;
	
	void Upgrade(int CurrentLevel);
	int CalculateIncreaseAmount(int Attribute);

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
	void GainExperience(int ExperienceAmount);
	
	UInventoryActorComponent* InventoryComponent;
	UInventoryWidget* InventoryWidget;
	
	void ToggleInventory();
	
	AFood* GetItemAtIndex(int32 Index);
	void DeleteItemAtIndex(int32 Index);
	bool AddItem(AFood* NewItem);
	void UseItem(int32 Index);
	
	void AddCollectibleFood(APickupFood* Food);
	void RemoveCollectibleFood(APickupFood* Food);
	
	float Damage = 10;
	float Health = 100;
	float MaxHealth = 100;
	float Experience = 0;
	float MaxExperienceLevel = 100;
	int Level = 0;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerUI> PlayerUIClass;
	UPROPERTY(EditAnywhere)
	UPlayerUI* PlayerUI;

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

