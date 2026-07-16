// Copyright Epic Games, Inc. All Rights Reserved.


#include "D4_HD3_CustomProjectPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "D4_HD3_CustomProject.h"
#include "D4_HD3_CustomProjectCharacter.h"
#include "InventoryWidget.h"
#include "Widgets/Input/SVirtualJoystick.h"

void AD4_HD3_CustomProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogD4_HD3_CustomProject, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AD4_HD3_CustomProjectPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

bool AD4_HD3_CustomProjectPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

void AD4_HD3_CustomProjectPlayerController::AttachUIWidget(ACharacter* NewPlayerCharacter)
{
	if (AD4_HD3_CustomProjectCharacter* PlayerCharacter = Cast<AD4_HD3_CustomProjectCharacter>(NewPlayerCharacter))
	{
		if (StarvationUIClass)
		{
			if (!StarvationUI)
			{
				
				StarvationUI = CreateWidget<UStarvationUI>(this, StarvationUIClass);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No value assigned to StarvationUIClass in %s"), *GetName());
		}
		
		if (CompanionStarvationUIClass)
		{
			if (!CompanionStarvationUI)
			{
				CompanionStarvationUI = CreateWidget<UCompanionStarvationUI>(this, CompanionStarvationUIClass);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No value assigned to CompanionStarvationUIClass in %s"), *GetName());
		}
		
		if (InventoryWidgetClass)
		{
			if (!InventoryWidget)
			{
				InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryWidgetClass);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No value assigned to InventoryWidgetClass in %s"), *GetName());
		}
	
		if (PlayerUIClass)
		{
			if (!PlayerUI)
			{
				PlayerUI = CreateWidget<UPlayerUI>(this, PlayerUIClass);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No value assigned to PlayerUIClass in %s"), *GetName());
		}
	
		if (DeathUIClass)
		{
			if (!DeathUI)
			{
				DeathUI = CreateWidget<UDeathUI>(this, DeathUIClass);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No value assigned to DeathUIClass in %s"), *GetName());
		}
		
		if (StarvationUI)
		{
			PlayerCharacter->StarvationUI = StarvationUI;
		}
		
		if (CompanionStarvationUI)
		{
			PlayerCharacter->CompanionStarvationUI = CompanionStarvationUI;
		}
		
		if (InventoryWidget)
		{
			PlayerCharacter->InventoryWidget = InventoryWidget;
			InventoryWidget->Owner = PlayerCharacter;
		}
		
		if (PlayerUI)
		{
			PlayerCharacter->PlayerUI = PlayerUI;
			PlayerUI->Player = PlayerCharacter;
		}
		
		if (DeathUI)
		{
			PlayerCharacter->DeathUI = DeathUI;
			DeathUI->Owner = PlayerCharacter;
		}
	}
}
