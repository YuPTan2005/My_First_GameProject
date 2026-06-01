// Copyright Epic Games, Inc. All Rights Reserved.

#include "D4_HD3_CustomProjectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "D4_HD3_CustomProject.h"
#include "Edible.h"
#include "Food.h"

AD4_HD3_CustomProjectCharacter::AD4_HD3_CustomProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
	GetCharacterMovement()->MaxWalkSpeed = 800;
	
	GetCharacterMovement()->NavAgentProps.bCanFly = true;
	this->GetCharacterMovement()->BrakingDecelerationFlying = 2000;
	this->GetCharacterMovement()->MaxFlySpeed = 1000;
	
	InventoryComponent = CreateDefaultSubobject<UInventoryActorComponent>(TEXT("Inventory Component"));
	bIsInventoryOpen = false;
}

void AD4_HD3_CustomProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AD4_HD3_CustomProjectCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AD4_HD3_CustomProjectCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AD4_HD3_CustomProjectCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AD4_HD3_CustomProjectCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AD4_HD3_CustomProjectCharacter::Look);
		
		EnhancedInputComponent->BindAction(CollectAction, ETriggerEvent::Started, this, &AD4_HD3_CustomProjectCharacter::Collect);
		
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AD4_HD3_CustomProjectCharacter::ToggleInventory);
	}
	else
	{
		UE_LOG(LogD4_HD3_CustomProject, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AD4_HD3_CustomProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (InventoryWidgetClass)
		{
			InventoryWidget = CreateWidget<UInventoryWidget>(PlayerController, InventoryWidgetClass);
			InventoryWidget->Owner = this;
		}
	}
}

void AD4_HD3_CustomProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AD4_HD3_CustomProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AD4_HD3_CustomProjectCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FRotator PitchYawRotation(Rotation.Pitch, Rotation.Yaw, 0);

		// get forward vector
		const FVector WalkingForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// get right vector 
		const FVector WalkingRightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		const FVector FlyingForwardDirection = FRotationMatrix(PitchYawRotation).GetUnitAxis(EAxis::X);
		const FVector FlyingRightDirection = FRotationMatrix(PitchYawRotation).GetUnitAxis(EAxis::Y);
		
		switch (GetCharacterMovement()->MovementMode)
		{
		case MOVE_Falling:
			AddMovementInput(WalkingForwardDirection, Forward);
			AddMovementInput(WalkingRightDirection, Right);
			break;
		case MOVE_Walking:
			AddMovementInput(WalkingForwardDirection, Forward);
			AddMovementInput(WalkingRightDirection, Right);
			break;
		case MOVE_Flying:
			AddMovementInput(FlyingForwardDirection, Forward);
			AddMovementInput(FlyingRightDirection, Right);
			break;
		}
	}
}

void AD4_HD3_CustomProjectCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AD4_HD3_CustomProjectCharacter::DoJumpStart()
{
	// signal the character to jump
	if (StateNumber <= MaxStateNumber)
	{
		StateNumber++;
		
		Jump();
		
		switch (StateNumber) {
		case 1:
			break;
		case 2:
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			bUseControllerRotationYaw = true;
			bUseControllerRotationPitch = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
			break;
		case 3:
			GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			bUseControllerRotationYaw = false;
			bUseControllerRotationPitch = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			ResetState();
			break;
		}
	}
}

void AD4_HD3_CustomProjectCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AD4_HD3_CustomProjectCharacter::Collect()
{
	if (CollectibleFood.Num() > 0 && !InventoryComponent->IsFull())
	{
		APickupFood* PickupFood = CollectibleFood[0];
		AFood* FoodToAdd;
		if (!PickupFood->Food)
		{
			FoodToAdd = NewObject<AFood>();
		}
		else
		{
			FoodToAdd = CollectibleFood[0]->Food;
		}
		if (AddItem(FoodToAdd))
		{
			CollectibleFood.RemoveAt(0);
			PickupFood->Collected();
		}
	}
}

void AD4_HD3_CustomProjectCharacter::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other,
                                               class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal,
                                               FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	
	if (HitNormal.Z >= GetCharacterMovement()->GetWalkableFloorZ())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		bUseControllerRotationYaw = false;
		bUseControllerRotationPitch = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		ResetState();
	}
}

void AD4_HD3_CustomProjectCharacter::ResetState()
{
	StateNumber = 0;
}

void AD4_HD3_CustomProjectCharacter::Eat(IEdible* Food)
{
	Food->EatenBy_Implementation(this);
}

void AD4_HD3_CustomProjectCharacter::GainExperience(int ExperienceAmount)
{
	Experience += ExperienceAmount;
	if (Experience >= MaxExperienceLevel)
	{
		Experience -= MaxExperienceLevel;
		Upgrade(Level);
	}
}

void AD4_HD3_CustomProjectCharacter::Upgrade(int CurrentLevel)
{
	Level++;
	Damage += CalculateIncreaseAmount(Damage);
	MaxHealth += CalculateIncreaseAmount(MaxHealth);
	Health += MaxHealth;
}

// Function to calculate the upgrade degree, larger level has smaller degree
int AD4_HD3_CustomProjectCharacter::CalculateIncreaseAmount(int Attribute)
{
	return Attribute * UpgradeFactor * (1 / (2 ^ Level));
}

AFood* AD4_HD3_CustomProjectCharacter::GetItemAtIndex(int32 Index)
{
	return InventoryComponent->GetItemAtIndex(Index);
}

void AD4_HD3_CustomProjectCharacter::DeleteItemAtIndex(int32 Index)
{
	InventoryComponent->DeleteItemAtIndex(Index);
	InventoryWidget->RefreshInventory(InventoryComponent->GetAllItems());
}

bool AD4_HD3_CustomProjectCharacter::AddItem(AFood* NewItem)
{
	if (InventoryComponent->AddItem(NewItem))
	{
		InventoryWidget->RefreshInventory(InventoryComponent->GetAllItems());
		return true;
	}
	return false;
}

void AD4_HD3_CustomProjectCharacter::UseItem(int32 Index)
{
	InventoryComponent->UseItemAtIndex(Index, this);
	InventoryWidget->RefreshInventory(InventoryComponent->GetAllItems());
}

void AD4_HD3_CustomProjectCharacter::ToggleInventory()
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)
		return;
	
	if (bIsInventoryOpen) {
		InventoryWidget->RemoveFromParent();
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
	else
	{
		InventoryWidget->AddToViewport();
		InventoryWidget->RefreshInventory(InventoryComponent->GetAllItems());
		PlayerController->SetShowMouseCursor(true);
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
		PlayerController->SetInputMode(InputMode);
	}
	
	bIsInventoryOpen = !bIsInventoryOpen;
}

void AD4_HD3_CustomProjectCharacter::AddCollectibleFood(APickupFood* Food)
{
	CollectibleFood.Add(Food);
}

void AD4_HD3_CustomProjectCharacter::RemoveCollectibleFood(APickupFood* Food)
{
	CollectibleFood.RemoveSingle(Food);
}
