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
#include "D4_HD3_CustomProjectGameMode.h"
#include "DeathUI.h"
#include "Edible.h"
#include "Enemy.h"
#include "Food.h"
#include "Kismet/GameplayStatics.h"

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
		
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AD4_HD3_CustomProjectCharacter::Attack);
		
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AD4_HD3_CustomProjectCharacter::Dash);
	}
	else
	{
		UE_LOG(LogD4_HD3_CustomProject, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AD4_HD3_CustomProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (InventoryComponentClass)
	{
		InventoryComponent = NewObject<UInventoryActorComponent>(this, InventoryComponentClass);
        
		if (InventoryComponent)
		{
			InventoryComponent->RegisterComponent();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No value assigned to inventory component class in %s"), *GetName());
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
			Cast<AD4_HD3_CustomProjectGameMode>(GameMode)->RespawnPlayer(this);
		}
	}
}

void AD4_HD3_CustomProjectCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	PlayerController = Cast<AD4_HD3_CustomProjectPlayerController>(NewController);
	if (PlayerController)
	{
		PlayerController->AttachUIWidget(this);
		PlayerUI->UpdateValues();
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
	if (InventoryComponent->GetHasBackpack() && CollectibleFood.Num() > 0)
	{
		APickupFood* PickupFood = CollectibleFood[0];
		AFood* FoodToAdd;
		if (!PickupFood->Food)
		{
			FoodToAdd = NewObject<AFood>();
		}
		else
		{
			FoodToAdd = PickupFood->Food;
		}
		
		bool AddFoodSuccess = true;
		if (AddItem(FoodToAdd))
		{
			CollectibleFood.RemoveSingle(PickupFood);
			if (Companion)
			{
				Companion->RemoveCollectibleFood_Implementation(PickupFood);
			}
			PickupFood->Collected(this);
		}
		else
		{
			AddFoodSuccess = false;
			PickupFood->UnCollected();
		}
		
		if (ViewportInfoUIClass)
		{
			UViewportInfoUI* ViewportInfoUI = CreateWidget<UViewportInfoUI>(GetGameInstance(), ViewportInfoUIClass);
			
			FVector2D ViewportInfoUILocation;
			UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), 
				PickupFood->GetActorLocation(), ViewportInfoUILocation);
			ViewportInfoUI->SetStartLocation(ViewportInfoUILocation);
			
			if (AddFoodSuccess)
			{
				ViewportInfoUI->SetDisplayText(FoodSuccessCollectedText);
				ViewportInfoUI->SetColorAndOpacity(FLinearColor(0.04f, 0.8f, 0.48f));
			}
			else
			{
				ViewportInfoUI->SetDisplayText(FoodFailCollectedText);
				ViewportInfoUI->SetColorAndOpacity(FLinearColor(1.0f, 0.25f, 0.38f));
			}
			
			ViewportInfoUI->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No value assigned to ViewportInfoUIClass in %s"), *GetName());
		}
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

void AD4_HD3_CustomProjectCharacter::GainExperience(float ExperienceAmount)
{
	Experience += ExperienceAmount;
	if (Experience >= MaxExperienceLevel)
	{
		Experience -= MaxExperienceLevel;
		Upgrade();
	}
}

void AD4_HD3_CustomProjectCharacter::GainStarvation(float StarvationAmount)
{
	StarvationValue += StarvationAmount;
	if (StarvationValue > MaxStarvationValue)
	{
		StarvationValue = MaxStarvationValue;
	}
}

void AD4_HD3_CustomProjectCharacter::Upgrade()
{
	Damage += CalculateIncreaseAmount(Damage);
	MaxHealth += 20;
	Health = MaxHealth;
	Level += 1;
}

// Function to calculate the upgrade degree, larger level has smaller degree
int AD4_HD3_CustomProjectCharacter::CalculateIncreaseAmount(float Attribute)
{
	return FMath::CeilToInt(Attribute * UpgradeFactor * (1.0f / FMath::Pow(2, Level)));
}

void AD4_HD3_CustomProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (PlayerUI)
	{
		PlayerUI->UpdateValues();
	}
	
	if (StarvationValue < 0 && !bIsCountDownCalled)
	{
		bIsCountDownCalled = true;
		DeathCountDown();
	}
	else if (bIsStarvationDecrement && StarvationValue >= 0)
	{
		StarvationValue += StarvationDecrementValue * DeltaSeconds;
		if (bIsCountDownCalled)
		{
			GetWorldTimerManager().ClearTimer(DeathTimerHandle);
			bIsCountDownCalled = false;
			StarvationUI->RemoveFromParent();
		}
	}
	
	if (AttackTimer < AttackCoolDown)
	{
		AttackTimer += DeltaSeconds;
	}
	
	if (bCanDash)
	{
		DashTimer += DeltaSeconds;
	}
}

void AD4_HD3_CustomProjectCharacter::DeathCountDown()
{
	GetWorldTimerManager().SetTimer(
		DeathTimerHandle,
		this,
		&AD4_HD3_CustomProjectCharacter::Dead,
		10.0f,
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
	bIsDead = true;
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	GetWorldTimerManager().SetTimer(
		ShowDeathUITimer,
		this,
		&AD4_HD3_CustomProjectCharacter::ShowDeathUI,
		2.0f,
		false
		);
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->SetShowMouseCursor(true);
	}
}

void AD4_HD3_CustomProjectCharacter::ShowDeathUI()
{
	if (DeathUI)
	{
		DeathUI->AddToViewport();
		DeathUI->Owner = this;
		if (PlayerController)
		{
			PlayerController->SetShowMouseCursor(true);
		}
	}
}

AFood* AD4_HD3_CustomProjectCharacter::GetItemAtIndex(int32 Index)
{
	if (InventoryComponent->GetHasBackpack())
	{
		return InventoryComponent->GetItemAtIndex(Index);
	}
	
	return nullptr;
}

void AD4_HD3_CustomProjectCharacter::DeleteItemAtIndex(int32 Index)
{
	if (InventoryComponent->GetHasBackpack())
	{
		InventoryComponent->DeleteItemAtIndex(Index);
		InventoryWidget->RefreshInventory(InventoryComponent->GetAllItems());
	}
}

bool AD4_HD3_CustomProjectCharacter::AddItem(AFood* NewItem)
{
	if (InventoryComponent->GetHasBackpack() && InventoryComponent->AddItem(NewItem))
	{
		InventoryWidget->RefreshInventory(InventoryComponent->GetAllItems());
		return true;
	}
	return false;
}

void AD4_HD3_CustomProjectCharacter::UseItem(int32 Index)
{
	if (InventoryComponent->GetHasBackpack())
	{
		InventoryComponent->UseItemAtIndex(Index, this);
		InventoryWidget->RefreshInventory(InventoryComponent->GetAllItems());
		PlayerUI->UpdateValues();
	}
}

void AD4_HD3_CustomProjectCharacter::AddCollectibleFood_Implementation(APickupFood* Food)
{
	if (InventoryComponent->GetHasBackpack())
	{
		CollectibleFood.Add(Food);
	}
}

void AD4_HD3_CustomProjectCharacter::RemoveCollectibleFood_Implementation(APickupFood* Food)
{
	if (InventoryComponent->GetHasBackpack())
	{
		CollectibleFood.Remove(Food);
	}
}

void AD4_HD3_CustomProjectCharacter::ToggleInventory()
{
	if (PlayerController && InventoryComponent->GetHasBackpack())
	{
		if (bIsInventoryOpen) 
		{
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
}

bool AD4_HD3_CustomProjectCharacter::IsDead_Implementation()
{
	return bIsDead;
}

void AD4_HD3_CustomProjectCharacter::SetIsDead(bool NewValue)
{
	bIsDead = NewValue;
}

bool AD4_HD3_CustomProjectCharacter::GetIsStarvationDecrement()
{
	return bIsStarvationDecrement;
}

void AD4_HD3_CustomProjectCharacter::SetIsStarvationDecrement(bool NewValue)
{
	bIsStarvationDecrement = NewValue;
}

bool AD4_HD3_CustomProjectCharacter::GetCanDash()
{
	return bCanDash;
}

void AD4_HD3_CustomProjectCharacter::SetCanDash(bool NewValue)
{
	bCanDash = NewValue;
}

float AD4_HD3_CustomProjectCharacter::GetDashSpeed()
{
	return DashSpeed;
}

void AD4_HD3_CustomProjectCharacter::SetDashSpeed(float NewValue)
{
	DashSpeed = NewValue;
}

float AD4_HD3_CustomProjectCharacter::GetDashTimer()
{
	return DashTimer;
}

void AD4_HD3_CustomProjectCharacter::SetDashTimer(float NewValue)
{
	DashTimer = NewValue;
}

float AD4_HD3_CustomProjectCharacter::GetDashCoolDown()
{
	return DashCoolDown;
}

void AD4_HD3_CustomProjectCharacter::SetDashCoolDown(float NewValue)
{
	DashCoolDown = NewValue;
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

void AD4_HD3_CustomProjectCharacter::Attack()
{
	if (AttackTimer >= AttackCoolDown)
	{
		AttackTimer = 0;
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			if (AttackAnims)
			{
				int AnimIndex = FMath::RandRange(0, AttackAnims->GetNumSections() - 1);
				AnimInstance->Montage_Play(AttackAnims);
				AnimInstance->Montage_JumpToSection(AttackAnims->GetSectionName(AnimIndex), AttackAnims);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("%s doesn't have attack montage"), *GetName());
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
					Execute_DealDamage(HitActor, Damage, this);
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
	}
}
