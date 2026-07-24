// Copyright Epic Games, Inc. All Rights Reserved.

#include "D4_HD3_CustomProjectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "CompanionStarvationUI.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "D4_HD3_CustomProject.h"
#include "D4_HD3_CustomProjectGameMode.h"
#include "DeathUI.h"
#include "Edible.h"
#include "Enemy.h"
#include "Food.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

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
	this->GetCharacterMovement()->BrakingDecelerationFlying = OriginalFlyBrake;
	this->GetCharacterMovement()->MaxFlySpeed = 1000;
	
	WindAmbientAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("WindAmbientAudioComp"));
	WindAmbientAudioComp->SetupAttachment(RootComponent);
	WindAmbientAudioComp->bAutoActivate = false;
	
	BirdAmbientAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("BirdAmbientAudioComp"));
	BirdAmbientAudioComp->SetupAttachment(RootComponent);
	BirdAmbientAudioComp->bAutoActivate = false;
	
	bIsInventoryOpen = false;
	bIsWeaponInventoryOpen = false;
	WeaponUsingIndex = -1;
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
		EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Started, this, &AD4_HD3_CustomProjectCharacter::Pickup);
		EnhancedInputComponent->BindAction(EatAction, ETriggerEvent::Started, this, &AD4_HD3_CustomProjectCharacter::Eat);
		EnhancedInputComponent->BindAction(FeedAction, ETriggerEvent::Started, this, &AD4_HD3_CustomProjectCharacter::Feed);
		
		EnhancedInputComponent->BindAction(FoodInventoryAction, ETriggerEvent::Started, this, &AD4_HD3_CustomProjectCharacter::ToggleInventory);
		EnhancedInputComponent->BindAction(WeaponInventoryAction, ETriggerEvent::Started, this, &AD4_HD3_CustomProjectCharacter::ToggleWeaponInventory);
		
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AD4_HD3_CustomProjectCharacter::Attack);
		
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AD4_HD3_CustomProjectCharacter::Dash);
		
		EnhancedInputComponent->BindAction(SwapWeaponAction, ETriggerEvent::Started, this, &AD4_HD3_CustomProjectCharacter::SwapWeapon);
	}
	else
	{
		UE_LOG(LogD4_HD3_CustomProject, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AD4_HD3_CustomProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UpgradeDamageValue = Damage;
	
	if (FoodInventoryComponentClass)
	{
		FoodInventoryComponent = NewObject<UFoodInventoryActorComponent>(this, FoodInventoryComponentClass);
        
		if (FoodInventoryComponent)
		{
			FoodInventoryComponent->RegisterComponent();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No value assigned to FoodInventoryComponentClass in %s"), *GetName());
	}
	
	if (WeaponInventoryComponentClass)
	{
		WeaponInventoryComponent = NewObject<UWeaponInventoryActorComponent>(this, WeaponInventoryComponentClass);
        
		if (WeaponInventoryComponent)
		{
			WeaponInventoryComponent->RegisterComponent();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No value assigned to WeaponInventoryComponentClass in %s"), *GetName());
	}
	
	if (CompanionClass)
	{
		float CharacterHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.0f;
		float CharacterRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
		FVector CompanionSpawnLocation = GetActorLocation() + 
				FVector(CharacterRadius + 100.0f, CharacterRadius + 100.0f, CharacterHeight - 150.0f);
		FRotator CompanionSpawnRotation = GetActorRotation();
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		Companion = GetWorld()->SpawnActor<ACompanion>(
			CompanionClass, 
			CompanionSpawnLocation, 
			CompanionSpawnRotation, 
			SpawnParams
		);
		
		Companion->SetCompanionOwner(this);
		InitialiseCompanionUI();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Companion class attached to %s"), *GetName())
	}
}

void AD4_HD3_CustomProjectCharacter::Destroyed()
{
	Super::Destroyed();
	
	if (UWorld* World = GetWorld())
	{
		if (AGameModeBase* GameMode = World->GetAuthGameMode())
		{
			if (Companion)
			{
				Companion->Destroy();
			}
			
			if (AD4_HD3_CustomProjectGameMode* MainCharacterGameMode = Cast<AD4_HD3_CustomProjectGameMode>(GameMode))
			{
				MainCharacterGameMode->RespawnPlayer(this);
			}
		}
	}
}

void AD4_HD3_CustomProjectCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!NewController) return;
	
	PlayerController = Cast<AD4_HD3_CustomProjectPlayerController>(NewController);
	if (PlayerController)
	{
		PlayerController->AttachUIWidget(this);
		if (PlayerUI)
		{
			PlayerUI->UpdatePlayerValues();
			PlayerUI->AddToViewport();
		}
		InitialiseCompanionUI();
	}
}

void AD4_HD3_CustomProjectCharacter::InitialiseCompanionUI()
{
	if (PlayerUI)
	{
		PlayerUI->SetCompanion(Companion);
		PlayerUI->UpdateCompanionValues();
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
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AnimInstance->IsAnyMontagePlaying())
	{
		return; 
	}
	
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
	if (FoodInventoryComponent->GetHasBackpack() && CollectibleFood.Num() > 0)
	{
		APickupFood* PickupFood = CollectibleFood[0];
		const FVector PickupFoodLocation = PickupFood->GetActorLocation();
		
		bool AddFoodSuccess = false;
		
		if (!FoodInventoryComponent->IsFull())
		{
			AActor* ItemToAdd = PickupFood->PickedUp();
			AFood* FoodToAdd = Cast<AFood>(ItemToAdd);
			if (FoodToAdd && AddItem(FoodToAdd))
			{
				CollectibleFood.RemoveSingle(PickupFood);
				if (Companion)
				{
					Companion->RemoveCollectibleItem_Implementation(PickupFood);
				}
				PickupFood->Collected(this);
				AddFoodSuccess = true;
			}
		}
		
		if (!AddFoodSuccess)
		{
			PickupFood->UnCollected();
		}
	
		AddInfoUIToViewport(
			PickupFoodLocation, 
			AddFoodSuccess,
			FoodSuccessCollectedText,
			FLinearColor(0.04f, 0.8f, 0.48f),
			FoodFailCollectedText,
			FLinearColor(1.0f, 0.25f, 0.38f)
			);
	}
}

void AD4_HD3_CustomProjectCharacter::Eat()
{
	Eat(this, FoodEatenText);
}

void AD4_HD3_CustomProjectCharacter::Feed()
{
	if (Companion)
	{
		Eat(Companion, FoodFedText);
	}
}

void AD4_HD3_CustomProjectCharacter::Eat(AActor* Consumer, const FString& EatenText)
{
	if (Consumer->Implements<UFoodConsumer>() && !FoodInventoryComponent->GetHasBackpack() && EdibleFood.Num() > 0)
	{
		APickupFood* PickupFood = EdibleFood[0];
		if (!IsValid(PickupFood))
		{
			EdibleFood.RemoveAt(0);
			return; 
		}
		
		AActor* FoodToEat = PickupFood->PickedUp();
		const FVector PickupFoodLocation = PickupFood->GetActorLocation();
		
		if (FoodToEat->Implements<UEdible>() && Companion)
		{
			Execute_Eat(Consumer, FoodToEat);
			EdibleFood.RemoveSingle(PickupFood);
			Companion->RemoveCollectibleItem_Implementation(PickupFood);
			PickupFood->Destroy();
			
			AddInfoUIToViewport(
				PickupFoodLocation, 
				true,
				EatenText,
				FLinearColor(0.85f, 0.55f, 0.08f)
				);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Item in %s is not a type of food"), *PickupFood->GetName());
		}
	}
}

void AD4_HD3_CustomProjectCharacter::Pickup()
{
	if (CollectibleWeapon.Num() > 0)
	{
		bool AddWeaponSuccess = false;
		APickupWeapon* PickupWeapon = CollectibleWeapon[0];
		const FVector PickupWeaponLocation = PickupWeapon->GetActorLocation();
		
		if (!WeaponInventoryComponent->IsFull())
		{
			AActor* ItemToAdd = PickupWeapon->PickedUp();
			AWeapon* WeaponToAdd = Cast<AWeapon>(ItemToAdd);
			if (WeaponToAdd && AddWeapon(WeaponToAdd))
			{
				CollectibleWeapon.RemoveSingle(PickupWeapon);
				AddWeaponSuccess = true;
				PickupWeapon->Destroy();
			}
		}
		
		AddInfoUIToViewport(
			PickupWeaponLocation, 
			AddWeaponSuccess,
			WeaponSuccessCollectedText,
			FLinearColor(0.04f, 0.8f, 0.48f),
			WeaponFailCollectedText,
			FLinearColor(1.0f, 0.25f, 0.38f)
			);
	}
}

void AD4_HD3_CustomProjectCharacter::AddInfoUIToViewport(const FVector& ItemLocation, const bool CollectionResult,
	const FString& SuccessCollectionText, const FLinearColor SuccessCollectionColor, 
	TOptional<FString> FailCollectionText, TOptional<FLinearColor> FailCollectionColor) const
{
	if (ViewportInfoUIClass)
	{
		UViewportInfoUI* ViewportInfoUI = CreateWidget<UViewportInfoUI>(GetGameInstance(), ViewportInfoUIClass);
		
		FVector2D ViewportInfoUILocation;
		UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), 
			ItemLocation, ViewportInfoUILocation);
		ViewportInfoUI->SetStartLocation(ViewportInfoUILocation);
		
		if (CollectionResult)
		{
			ViewportInfoUI->SetDisplayText(SuccessCollectionText);
			ViewportInfoUI->SetColorAndOpacity(SuccessCollectionColor);
		}
		else
		{
			if (FailCollectionText.IsSet() && FailCollectionColor.IsSet())
			{
				ViewportInfoUI->SetDisplayText(FailCollectionText.GetValue());
				ViewportInfoUI->SetColorAndOpacity(FailCollectionColor.GetValue());
			}
		}
		
		ViewportInfoUI->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No value assigned to ViewportInfoUIClass in %s"), *GetName());
	}
}

void AD4_HD3_CustomProjectCharacter::Dash()
{
	if (bCanDash && DashTimer >= DashCoolDown)
	{
		UCharacterMovementComponent* MoveComponent = GetCharacterMovement();
		FVector DashDirection = GetActorForwardVector();
		DashTimer = 0.0f;
		
		if (GetCharacterMovement()->MovementMode == MOVE_Walking)
		{
			LaunchCharacter(DashDirection * DashSpeed, true, true);
		}
		else if (GetCharacterMovement()->MovementMode == MOVE_Flying)
		{
			MoveComponent->Velocity = DashDirection * DashSpeed;
			GetCharacterMovement()->BrakingDecelerationFlying = DashFlyBrake;
			
			GetWorldTimerManager().ClearTimer(FlyDashTimerHandle);
			GetWorldTimerManager().SetTimer(
				FlyDashTimerHandle, 
				this,
				&AD4_HD3_CustomProjectCharacter::RestoreFlyBrake, 
				0.5f, 
				false);
		}
	}
}

void AD4_HD3_CustomProjectCharacter::RestoreFlyBrake()
{
	GetCharacterMovement()->BrakingDecelerationFlying = OriginalFlyBrake;
}

void AD4_HD3_CustomProjectCharacter::SwapWeapon()
{
	if (WeaponInventoryComponent)
	{
		int8 WeaponInventorySize = WeaponInventoryComponent->GetInventoryItemSize();
		if (WeaponInventorySize != 0)
		{
			const int8 PreviousIndex = WeaponUsingIndex;
			
			int8 NextIndex = PreviousIndex + 1;
			if (NextIndex >= WeaponInventorySize)
			{
				NextIndex = -1;
			}
			
			if (PreviousIndex >= 0 && PreviousIndex < WeaponInventorySize)
			{
				UnuseWeapon(PreviousIndex);
			}
			
			if (NextIndex >= 0 && NextIndex < WeaponInventorySize)
			{
				UseWeapon(NextIndex);
			}
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

void AD4_HD3_CustomProjectCharacter::Eat_Implementation(AActor* Food)
{
	if (Food->Implements<UEdible>())
	{
		IEdible::Execute_EatenBy(Food, this);
	}
}

void AD4_HD3_CustomProjectCharacter::GainExperience(float ExperienceAmount)
{
	Experience += ExperienceAmount;
	
	if (Experience >= MaxExperience)
	{
		if (Level == MaxLevel)
		{
			Experience = MaxExperience;
		}
		else
		{
			Experience -= MaxExperience;
			Upgrade();
		}
	}
}

void AD4_HD3_CustomProjectCharacter::GainStarvation_Implementation(float StarvationAmount)
{
	StarvationValue += StarvationAmount;
	if (StarvationValue > MaxStarvationValue)
	{
		StarvationValue = MaxStarvationValue;
	}
	
	if (bIsCountDownCalled)
	{
		GetWorldTimerManager().ClearTimer(DeathTimerHandle);
		bIsCountDownCalled = false;
		StarvationUI->RemoveFromParent();
	}
}

void AD4_HD3_CustomProjectCharacter::FeedItem(const int8 Index) const
{
	if (FoodInventoryComponent->GetHasBackpack() && IsValid(Companion))
	{
		FoodInventoryComponent->UseItemAtIndex(Index, Companion);
		FoodInventoryWidget->RefreshInventory(FoodInventoryComponent->GetAllItems());
		Companion->UpdateStatus();
	}
}

void AD4_HD3_CustomProjectCharacter::ToggleWeaponInventory()
{
	if (PlayerController && WeaponInventoryWidget)
	{
		if (bIsWeaponInventoryOpen)
		{
			WeaponInventoryWidget->RemoveFromParent();
			PlayerController->SetShowMouseCursor(false);
			PlayerController->SetInputMode(FInputModeGameOnly());
			if (PlayerUI)
			{
				PlayerUI->SetWeaponUIVisibility(true);
			}
		}
		else
		{
			WeaponInventoryWidget->AddToViewport();
			WeaponInventoryWidget->RefreshInventory(WeaponInventoryComponent->GetAllItems());
			PlayerController->SetShowMouseCursor(true);
			PlayerController->SetInputMode(FInputModeGameAndUI());
			if (PlayerUI)
			{
				PlayerUI->SetWeaponUIVisibility(false);
			}
		}
		
		bIsWeaponInventoryOpen = !bIsWeaponInventoryOpen;
	}
}

AActor* AD4_HD3_CustomProjectCharacter::GetWeaponAtIndex(const int8 Index) const
{
	return WeaponInventoryComponent->GetItemAtIndex(Index);
}

void AD4_HD3_CustomProjectCharacter::DeleteWeaponAtIndex(const int8 Index) const
{
	WeaponInventoryComponent->DeleteItemAtIndex(Index);
	WeaponInventoryWidget->RefreshInventory(WeaponInventoryComponent->GetAllItems());
	if (PlayerUI)
	{
		PlayerUI->RemoveWeaponImage(Index);
		PlayerUI->ResetWeaponBorderColor(Index);
	}
}

bool AD4_HD3_CustomProjectCharacter::AddWeapon(AActor* NewItem) const
{
	if (WeaponInventoryComponent->AddItem(NewItem))
	{
		if (const AWeapon* NewWeapon = Cast<AWeapon>(NewItem))
		{
			if (PlayerUI) PlayerUI->SetNewWeaponImage(NewWeapon->GetWeaponImage());
		}
		WeaponInventoryWidget->RefreshInventory(WeaponInventoryComponent->GetAllItems());
		return true;
	}
	return false;
}

void AD4_HD3_CustomProjectCharacter::UseWeapon(const int8 Index)
{
	if (WeaponInventoryComponent->UseItemAtIndex(Index, this))
	{
		WeaponUsingIndex = Index;
		
		if (PlayerUI)
		{
			PlayerUI->SetWeaponBorderColor(Index, WeaponOnUsedColor);
		}
	
		if (WeaponInventoryWidget)
		{
			WeaponInventoryWidget->SetSelectedItemIndex(WeaponUsingIndex);
			WeaponInventoryWidget->SetButtonBorderColor(Index, WeaponOnUsedColor);
		}
	}
}

void AD4_HD3_CustomProjectCharacter::UnuseWeapon(const int8 WeaponIndex)
{
	if (PlayerUI)
	{
		PlayerUI->ResetWeaponBorderColor(WeaponIndex);
	}
	
	if (WeaponInventoryWidget)
	{
		WeaponInventoryWidget->ResetButtonBorderColor(WeaponIndex);
	}
	
	AActor* CurrentWeapon = GetWeaponAtIndex(WeaponIndex);
	if (AWeapon* WeaponToDisattach = Cast<AWeapon>(CurrentWeapon))
	{
		DisattachWeaponFromSocket(WeaponToDisattach);
	}
	
	WeaponUsingIndex = -1;
}

void AD4_HD3_CustomProjectCharacter::DisattachWeaponFromSocket(AWeapon* Weapon) const
{
	if (Weapon)
	{
		FDetachmentTransformRules DetachRules(
			EDetachmentRule::KeepWorld,
			true
		);
		
		Weapon->SetWeaponMeshVisibility(false);
		Weapon->DetachFromActor(DetachRules);
	}
}

void AD4_HD3_CustomProjectCharacter::AttachWeaponToSocket(AWeapon* Weapon, const FString& SocketName) const
{
	if (Weapon && GetMesh())
	{
		const FAttachmentTransformRules AttachRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			true
		);

		Weapon->AttachToComponent(GetMesh(), AttachRules, FName(SocketName));
		Weapon->SetWeaponMeshVisibility(true);
	}
}

void AD4_HD3_CustomProjectCharacter::Upgrade()
{
	if (Level <= MaxLevel)
	{
		UpgradeDamageValue = CalculateIncreaseAmount(UpgradeDamageValue);
		Damage += UpgradeDamageValue;
		MaxHealth += 20;
		Health = MaxHealth;
		MaxExperience += ExperienceUpgradeValue;
		Level++;
		
		if (Companion)
		{
			Companion->Upgrade();
		}
		
		PlayerUI->UpdateValues();
	}
}

// Function to calculate the upgrade degree, larger level has smaller degree
float AD4_HD3_CustomProjectCharacter::CalculateIncreaseAmount(float Attribute) const
{
	return FMath::CeilToInt(Attribute * DamageUpgradeFactor * (1.0f / FMath::Pow(2, Level)));
}

void AD4_HD3_CustomProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (StarvationValue < 0 && !bIsCountDownCalled && !bIsDead)
	{
		DeathCountDown();
	}
	else if (bIsStarvationDecrement && StarvationValue >= 0 && !bIsDead)
	{
		StarvationValue += StarvationDecrementValue * DeltaSeconds;
	}
	
	if (AttackTimer < AttackCoolDown)
	{
		AttackTimer += DeltaSeconds;
	}
	
	if (bCanDash)
	{
		DashTimer += DeltaSeconds;
	}
	
	if (PlayerUI)
	{
		PlayerUI->UpdatePlayerValues();
	}
}

void AD4_HD3_CustomProjectCharacter::DeathCountDown()
{
	bIsCountDownCalled = true;
	
	GetWorldTimerManager().SetTimer(
		DeathTimerHandle,
		this,
		&AD4_HD3_CustomProjectCharacter::Dead,
		CountDownTime,
		false
		);
	
	if (StarvationUI)
	{
		StarvationUI->AddToViewport();
		StarvationUI->TriggerAnimation();
	}
}

void AD4_HD3_CustomProjectCharacter::Dead()
{
	if (StarvationUI && StarvationUI->IsInViewport())
	{
		StarvationUI->RemoveFromParent();
	}
	if (CompanionStarvationUI && CompanionStarvationUI->IsInViewport())
	{
		CompanionStarvationUI->RemoveFromParent();
	}
	
	bIsDead = true;
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	
	if (PlayerController && DeathUI)
	{
		DeathUI->AddToViewport();
		DeathUI->Owner = this;
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(DeathUI->TakeWidget());
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
		
		GetWorldTimerManager().SetTimer(
		ShowDeathUITimer,
		this,
		&AD4_HD3_CustomProjectCharacter::ShowDeathUIWidget,
		2.0f,
		false
		);
	}
}

void AD4_HD3_CustomProjectCharacter::ShowDeathUIWidget() const
{
	if (DeathUI && DeathUI->IsInViewport())
	{
		DeathUI->ShowUIWidget();
	}
}

AActor* AD4_HD3_CustomProjectCharacter::GetItemAtIndex(const int8 Index) const
{
	if (FoodInventoryComponent->GetHasBackpack())
	{
		return FoodInventoryComponent->GetItemAtIndex(Index);
	}
	
	return nullptr;
}

void AD4_HD3_CustomProjectCharacter::DeleteItemAtIndex(const int8 Index) const
{
	if (FoodInventoryComponent->GetHasBackpack())
	{
		FoodInventoryComponent->DeleteItemAtIndex(Index);
		FoodInventoryWidget->RefreshInventory(FoodInventoryComponent->GetAllItems());
	}
}

bool AD4_HD3_CustomProjectCharacter::AddItem(AFood* NewItem) const
{
	if (FoodInventoryComponent->GetHasBackpack() && FoodInventoryComponent->AddItem(NewItem))
	{
		FoodInventoryWidget->RefreshInventory(FoodInventoryComponent->GetAllItems());
		return true;
	}
	return false;
}

void AD4_HD3_CustomProjectCharacter::UseItem(const int8 Index)
{
	if (FoodInventoryComponent->GetHasBackpack())
	{
		FoodInventoryComponent->UseItemAtIndex(Index, this);
		FoodInventoryWidget->RefreshInventory(FoodInventoryComponent->GetAllItems());
		PlayerUI->UpdatePlayerValues();
	}
}

void AD4_HD3_CustomProjectCharacter::AddCollectibleItem_Implementation(APickupItem* Item)
{
	if (APickupFood* Food = Cast<APickupFood>(Item))
	{
		if (FoodInventoryComponent->GetHasBackpack())
		{
			CollectibleFood.Add(Food);
		}
		else
		{
			EdibleFood.Add(Food);
		}
	}
	else if (APickupWeapon* Weapon = Cast<APickupWeapon>(Item))
	{
		CollectibleWeapon.Add(Weapon);
	}
}

void AD4_HD3_CustomProjectCharacter::RemoveCollectibleItem_Implementation(APickupItem* Item)
{
	if (APickupFood* Food = Cast<APickupFood>(Item))
	{
		if (FoodInventoryComponent->GetHasBackpack())
		{
			CollectibleFood.Remove(Food);
		}
		else
		{
			EdibleFood.Remove(Food);
		}
	}
}

void AD4_HD3_CustomProjectCharacter::ToggleInventory()
{
	if (PlayerController && FoodInventoryComponent && FoodInventoryComponent->GetHasBackpack())
	{
		if (bIsInventoryOpen) 
		{
			FoodInventoryWidget->RemoveFromParent();
			PlayerController->SetShowMouseCursor(false);
			PlayerController->SetInputMode(FInputModeGameOnly());
		}
		else
		{
			FoodInventoryWidget->AddToViewport();
			FoodInventoryWidget->RefreshInventory(FoodInventoryComponent->GetAllItems());
			PlayerController->SetShowMouseCursor(true);
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(FoodInventoryWidget->TakeWidget());
			PlayerController->SetInputMode(InputMode);
		}
		
		bIsInventoryOpen = !bIsInventoryOpen;
	}
}

bool AD4_HD3_CustomProjectCharacter::IsDead_Implementation()
{
	return bIsDead;
}

void AD4_HD3_CustomProjectCharacter::SetIsDead(const bool NewValue)
{
	bIsDead = NewValue;
}

float AD4_HD3_CustomProjectCharacter::GetCurrentLevel() const
{
	return Level;
}

void AD4_HD3_CustomProjectCharacter::SetCurrentLevel(const float NewValue)
{
	Level = NewValue;
}

float AD4_HD3_CustomProjectCharacter::GetExperience() const
{
	return Experience;
}

void AD4_HD3_CustomProjectCharacter::SetExperience(const float NewValue)
{
	Experience = NewValue;
}

float AD4_HD3_CustomProjectCharacter::GetMaxExperience() const
{
	return MaxExperience;
}

void AD4_HD3_CustomProjectCharacter::SetMaxExperience(const float NewValue)
{
	MaxExperience = NewValue;
}

float AD4_HD3_CustomProjectCharacter::GetCurrentHealth_Implementation() const
{
	return Health;
}

void AD4_HD3_CustomProjectCharacter::SetCurrentHealth_Implementation(const float NewValue)
{
	Health = NewValue;
}

float AD4_HD3_CustomProjectCharacter::GetMaxHealth_Implementation() const
{
	return MaxHealth;
}

void AD4_HD3_CustomProjectCharacter::SetMaxHealth_Implementation(const float NewValue)
{
	MaxHealth = NewValue;
}

float AD4_HD3_CustomProjectCharacter::GetStarvationValue() const
{
	return StarvationValue;
}

void AD4_HD3_CustomProjectCharacter::SetStarvationValue(const float NewValue)
{
	StarvationValue = NewValue;
}

float AD4_HD3_CustomProjectCharacter::GetMaxStarvationValue() const
{
	return MaxStarvationValue;
}

void AD4_HD3_CustomProjectCharacter::SetMaxStarvationValue(const float NewValue)
{
	MaxStarvationValue = NewValue;
}

bool AD4_HD3_CustomProjectCharacter::GetIsStarvationDecrement() const
{
	return bIsStarvationDecrement;
}

void AD4_HD3_CustomProjectCharacter::SetIsStarvationDecrement(const bool NewValue)
{
	bIsStarvationDecrement = NewValue;
}

bool AD4_HD3_CustomProjectCharacter::GetCanDash() const
{
	return bCanDash;
}

void AD4_HD3_CustomProjectCharacter::SetCanDash(const bool NewValue)
{
	bCanDash = NewValue;
}

float AD4_HD3_CustomProjectCharacter::GetDashSpeed() const
{
	return DashSpeed;
}

void AD4_HD3_CustomProjectCharacter::SetDashSpeed(const float NewValue)
{
	DashSpeed = NewValue;
}

float AD4_HD3_CustomProjectCharacter::GetDashTimer() const
{
	return DashTimer;
}

void AD4_HD3_CustomProjectCharacter::SetDashTimer(const float NewValue)
{
	DashTimer = NewValue;
}

float AD4_HD3_CustomProjectCharacter::GetDashCoolDown() const
{
	return DashCoolDown;
}

void AD4_HD3_CustomProjectCharacter::SetDashCoolDown(const float NewValue)
{
	DashCoolDown = NewValue;
}

bool AD4_HD3_CustomProjectCharacter::GetHasBackpack() const
{
	return FoodInventoryComponent->GetHasBackpack();
}

void AD4_HD3_CustomProjectCharacter::SetHasBackpack(const bool NewValue) const
{
	FoodInventoryComponent->SetHasBackpack(NewValue);
}

ACompanion* AD4_HD3_CustomProjectCharacter::GetCompanion() const
{
	return Companion;
}

void AD4_HD3_CustomProjectCharacter::SetCompanion(ACompanion* NewCompanion)
{
	Companion = NewCompanion;
}

bool AD4_HD3_CustomProjectCharacter::GetIsFoodInventoryFull() const
{
	return FoodInventoryComponent->IsFull();
}

void AD4_HD3_CustomProjectCharacter::DealDamage_Implementation(float DamageTook, AActor* DamagedBy)
{
	Health = FMath::Clamp(Health - DamageTook, 0, MaxHealth);
	if (Companion)
	{
		if (AEnemy* DamagedByEnemy = Cast<AEnemy>(DamagedBy))
		{
			Companion->SetTargetEnemy(DamagedByEnemy);
		}
	}
	
	if (Health <= 0 && !bIsDead)
	{
		Dead();
	}
}

EGameTeam AD4_HD3_CustomProjectCharacter::GetTeam_Implementation()
{
	return EGameTeam::Players;
}

void AD4_HD3_CustomProjectCharacter::IncreaseDamageValue(const float DamageValue)
{
	Damage += DamageValue;
	if (PlayerUI)
	{
		PlayerUI->UpdatePlayerValues();
	}
}

void AD4_HD3_CustomProjectCharacter::Attack()
{
	if (AttackTimer >= AttackCoolDown)
	{
		AttackTimer = 0;
		
		FString WeaponName = "None";
		AActor* DamageActor = this;
		if (WeaponUsingIndex != -1)
		{
			AActor* Item = WeaponInventoryComponent->GetItemAtIndex(WeaponUsingIndex);
			if (Item && Item->Implements<UInventoryItem>())
			{
				DamageActor = Item;
				WeaponName = IInventoryItem::Execute_GetName(Item);
			}
		}
		
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			if (GetCharacterMovement()->MovementMode == MOVE_Flying)
			{
				GetCharacterMovement()->BrakingDecelerationFlying = DashFlyBrake;
			
				GetWorldTimerManager().ClearTimer(FlyDashTimerHandle);
				GetWorldTimerManager().SetTimer(
					FlyDashTimerHandle, 
					this, 
					&AD4_HD3_CustomProjectCharacter::RestoreFlyBrake, 
					0.3f, 
					false);
			}
			
			if (UAnimMontage* AttackMontageToPlay = *AttackAnims.Find(WeaponName))
			{
				int AnimIndex = FMath::RandRange(0, AttackMontageToPlay->GetNumSections() - 1);
				FName TargetSectionName = AttackMontageToPlay->GetSectionName(AnimIndex);
				AnimInstance->Montage_Play(AttackMontageToPlay);
				AnimInstance->Montage_JumpToSection(TargetSectionName, AttackMontageToPlay);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s doesn't have anim instance"), *GetName());
		}	
		
		
		TArray<FHitResult> HitResults;
		const FVector Start = GetActorLocation();
		const FVector End = Start + GetActorForwardVector() * AttackDistance;
		const FCollisionShape CubeShape = FCollisionShape::MakeBox(FVector(AttackDistance));
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		TraceParams.AddIgnoredActor(Companion);
		const bool bSweep = GetWorld()->SweepMultiByChannel(HitResults, End, End, GetActorQuat(), 
			ECC_WorldDynamic, CubeShape, TraceParams);

		TArray<AActor*> HitThisPunch;
			
		for (FHitResult HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor && !HitThisPunch.Contains(HitActor))
			{
				if (HitActor->Implements<UDamageable>())
				{
					HitThisPunch.Add(HitActor);
					Execute_DealDamage(HitActor, Damage, DamageActor);
				}
			}
		}
		
		if (!HitThisPunch.IsEmpty())
		{
			AActor* RandomActor = HitThisPunch[FMath::RandRange(0, HitThisPunch.Num()-1)];
			if (AEnemy* RandomEnemy = Cast<AEnemy>(RandomActor))
			{
				if (Companion)
				{
					Companion->SetTargetEnemy(RandomEnemy);
				}
			}
		}
		
		PlayAttackSound();
	}
}

void AD4_HD3_CustomProjectCharacter::PlayAttackSound()
{
	if (AttackSounds.IsEmpty())
	{
		return;
	}
	
	const int32 RandomIndex = FMath::RandRange(0, AttackSounds.Num() - 1);
	if (USoundBase* SelectedSound = AttackSounds[RandomIndex])
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			SelectedSound,
			GetActorLocation()
		);
	}
}

void AD4_HD3_CustomProjectCharacter::ShowCompanionStarvationUI() const
{
	if (CompanionStarvationUI && !bIsDead)
	{
		CompanionStarvationUI->AddToViewport();
		CompanionStarvationUI->TriggerAnimation();
	}
}

void AD4_HD3_CustomProjectCharacter::ClearCompanionStarvationUI() const
{
	if (CompanionStarvationUI && CompanionStarvationUI->IsInViewport())
	{
		CompanionStarvationUI->RemoveFromParent();
	}
}

void AD4_HD3_CustomProjectCharacter::OnCompanionDie() const
{
	ClearCompanionStarvationUI();
	if (PlayerUI)
	{
		PlayerUI->SetCompanion(nullptr);
		PlayerUI->UpdateCompanionValues();
	}
	if (FoodInventoryWidget) FoodInventoryWidget->CancelFeedButton();
}

void AD4_HD3_CustomProjectCharacter::PlayAmbientSound() const
{
	if (WindAmbientAudioComp)
	{
		WindAmbientAudioComp->Play();
		WindAmbientAudioComp->SetVolumeMultiplier(WindAudioMultiplier);
	}
	
	if (BirdAmbientAudioComp)
	{
		BirdAmbientAudioComp->Play();
		WindAmbientAudioComp->SetVolumeMultiplier(BirdAudioMultiplier);
	}
}
