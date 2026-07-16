// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleItem.h"

#include "NPCStatus.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADestructibleItem::ADestructibleItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	HealthWidgetComponent = CreateDefaultSubobject<UNPCStatusComponent>(TEXT("HealthBarComponent"));
	HealthWidgetComponent->SetupAttachment(RootComponent);

	HealthWidgetComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 600.0f)); 
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawSize(FVector2D(150.0f, 40.0f));
}

// Called when the game starts or when spawned
void ADestructibleItem::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
	bIsDead = false;
	bIsShowingUI = false;
	
	if (HealthWidgetComponent)
	{
		HealthWidgetComponent->InitWidget();
		
		if (UUserWidget* UserWidget = HealthWidgetComponent->GetUserWidgetObject())
		{
			if (UNPCStatus* StatusWidget = Cast<UNPCStatus>(UserWidget))
			{
				StatusWidget->BindingActor = this;
				StatusWidget->UpdateValues();
			}
		}
		
		HealthWidgetComponent->SetVisibility(false);
		CurrentOpacity = 0.0f;
		bIsShowingUI = false;
	}
	
}

void ADestructibleItem::StartRestoreHealth()
{
	GetWorldTimerManager().ClearTimer(HealthRestoreTimerHandle);
	
	GetWorldTimerManager().SetTimer(
		HealthRestoreTimerHandle,
		this,
		&ADestructibleItem::RestoreHealth,
		RestoreTickRate,
		true
		);
}

void ADestructibleItem::RestoreHealth()
{
	CurrentHealth += MaxHealth * RestoreHealthSpeed;
	
	if (CurrentHealth >= MaxHealth)
	{
		GetWorldTimerManager().ClearTimer(HealthRestoreTimerHandle);
		CurrentHealth = MaxHealth;
	}
	
	UpdateStatus();
}

void ADestructibleItem::ResetShowingTime()
{
	ShowTimeTracker = 0.0f;
	
	GetWorldTimerManager().ClearTimer(ShowTimerHandle);
	GetWorldTimerManager().ClearTimer(ScaleWidgetTimer);
	GetWorldTimerManager().ClearTimer(HealthRestoreTimerHandle);
	
	GetWorldTimerManager().SetTimer(
		ShowTimerHandle,
		this,
		&ADestructibleItem::ReduceShowingTime,
		TimeTrackerRate,
		true
	);
	
	GetWorldTimerManager().SetTimer(
		ScaleWidgetTimer,
		this,
		&ADestructibleItem::ScaleUIWidget,
		ScaleWidgetRate,
		true
	);
}

void ADestructibleItem::ReduceShowingTime()
{
	ShowTimeTracker += TimeTrackerRate;

	if (ShowTimeTracker >= UIShowingTime)
	{
		GetWorldTimerManager().ClearTimer(ShowTimerHandle);
		HideUI();
	}
}

void ADestructibleItem::ScaleUIWidget()
{
	if (ShowTimeTracker < UIShowingTime && HealthWidgetComponent)
	{
		UUserWidget* UserWidget = HealthWidgetComponent->GetUserWidgetObject();
		if (!UserWidget) return;
		
		if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
		{
			const float Distance = FVector::Dist(CameraManager->GetCameraLocation(), GetActorLocation());
			
			constexpr float ReferenceDistance = 1200.0f;
			constexpr float MinimumSafeDistance = 50.0f;
			const float SafeDistance = FMath::Max(Distance, MinimumSafeDistance);
			
			const float TargetScale = ReferenceDistance / SafeDistance;
			
			UserWidget->SetRenderScale(FVector2D(TargetScale, TargetScale));
		}
	}
	else if (ShowTimeTracker >= UIShowingTime)
	{
		GetWorldTimerManager().ClearTimer(ScaleWidgetTimer);
	}
}

void ADestructibleItem::ShowUI()
{
	StartFade(true);
}

void ADestructibleItem::HideUI()
{
	StartFade(false);
	StartRestoreHealth();
}

void ADestructibleItem::UpdateStatus() const
{
	if (HealthWidgetComponent && HealthWidgetComponent->GetUserWidgetObject())
	{
		if (UNPCStatus* StatusWidget = Cast<UNPCStatus>(HealthWidgetComponent->GetUserWidgetObject()))
		{
			StatusWidget->UpdateValues();
		}
	}
}

void ADestructibleItem::StartFade(bool bFadeIn)
{
	bTargetFadeIn = bFadeIn;
	
	if (bTargetFadeIn)
	{
		HealthWidgetComponent->SetVisibility(true);
	}
	
	GetWorldTimerManager().ClearTimer(FadeTimerHandle);
	GetWorldTimerManager().SetTimer(
		FadeTimerHandle,
		this,
		&ADestructibleItem::UpdateFade,
		FadeTickRate,
		true
	);
}

void ADestructibleItem::UpdateFade()
{
	if (bTargetFadeIn)
	{
		CurrentOpacity += FadeSpeed * FadeTickRate;
	}
	else
	{
		CurrentOpacity -= FadeSpeed * FadeTickRate;
	}

	CurrentOpacity = FMath::Clamp(CurrentOpacity, 0.0f, 1.0f);
	if (UUserWidget* UserWidget = HealthWidgetComponent->GetUserWidgetObject())
	{
		UserWidget->SetRenderOpacity(CurrentOpacity);
	}
	
	if ((bTargetFadeIn && CurrentOpacity >= 1.0f) || (!bTargetFadeIn && CurrentOpacity <= 0.0f))
	{
		GetWorldTimerManager().ClearTimer(FadeTimerHandle);
		if (!bTargetFadeIn)
		{
			HealthWidgetComponent->SetVisibility(false);
			bIsShowingUI = false;
		}
		else
		{
			bIsShowingUI = true;
		}
	}
}

float ADestructibleItem::GetCurrentHealth_Implementation()
{
	return CurrentHealth;
}

float ADestructibleItem::GetMaxHealth_Implementation()
{
	return MaxHealth;
}

void ADestructibleItem::DealDamage_Implementation(float DamageTaken, AActor* DamagedBy)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageTaken, 0.0f, MaxHealth);
	UpdateStatus();
	
	if (!bIsShowingUI)
	{
		ShowUI();
	}
	ResetShowingTime();
	
	if (CurrentHealth <= 0 && !bIsDead)
	{
		bIsDead = true;
		OnDestructed();
	}
}

bool ADestructibleItem::IsDead_Implementation()
{
	return bIsDead;
}

EGameTeam ADestructibleItem::GetTeam_Implementation()
{
	return EGameTeam::Enemies;
}

void ADestructibleItem::OnDestructed()
{
	if (HealthWidgetComponent)
	{
		HealthWidgetComponent->SetVisibility(false);
	}
	
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

// Called every frame
void ADestructibleItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

