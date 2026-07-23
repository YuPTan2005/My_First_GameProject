// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnLoadingUI.h"

void URespawnLoadingUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	LoadingAndFadeRate = 0.016f;
	LoadingTracker = 0.0f;
	
	BackgroundOpacity = 1.0f;
	BackgroundAcceptanceFade = 0.01f;
	
	if (BackgroundFadeTime <= 0.0f)
	{
		BackgroundFadeTime = 1.0f;
	}
    
	if (LoadingTime <= 0.0f)
	{
		LoadingTime = 2.0f;
	}
}

void URespawnLoadingUI::NativeDestruct()
{
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(LoadingTimerHandle);
		World->GetTimerManager().ClearTimer(BackgroundFadeTimerHandle);
	}

	Super::NativeDestruct();
}

void URespawnLoadingUI::StartBackgroundTransition()
{
	GetWorld()->GetTimerManager().SetTimer(
		BackgroundFadeTimerHandle,
		this,
		&URespawnLoadingUI::FadeBackground,
		LoadingAndFadeRate,
		true
	);
}

void URespawnLoadingUI::FadeBackground()
{
	BackgroundOpacity -= LoadingAndFadeRate / BackgroundFadeTime;
	
	if (BackgroundOpacity <= BackgroundAcceptanceFade)
	{
		BackgroundOpacity = 0.0f;
		Background->SetOpacity(0.0f);
		
		GetWorld()->GetTimerManager().ClearTimer(BackgroundFadeTimerHandle);
		
		if (APlayerController* Controller = GetWorld()->GetFirstPlayerController())
		{
			Controller->SetInputMode(FInputModeGameOnly());
			Controller->SetShowMouseCursor(false);
		}
		
		RemoveFromParent();
		return;
	}
	
	Background->SetOpacity(BackgroundOpacity);
}

void URespawnLoadingUI::LoadProgress()
{
	LoadingTracker += LoadingAndFadeRate;
	
	if (LoadingTracker >= LoadingTime)
	{
		LoadingTracker = LoadingTime;
		GetWorld()->GetTimerManager().ClearTimer(LoadingTimerHandle);
		StartBackgroundTransition();
	}
	
	if (LoadingProgress && LoadingTime > 0.0f)
	{
		LoadingProgress->SetPercent(LoadingTracker / LoadingTime);
	}
}

void URespawnLoadingUI::StartLoading()
{
	LoadingTracker = 0.0f;
	
	GetWorld()->GetTimerManager().SetTimer(
		LoadingTimerHandle,
		this,
		&URespawnLoadingUI::LoadProgress,
		LoadingAndFadeRate,
		true
	);
}
