// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "RespawnLoadingUI.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API URespawnLoadingUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UProgressBar* LoadingProgress;
	UPROPERTY(meta=(BindWidget))
	UImage* Background;
	
	FTimerHandle LoadingTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LoadingTime = 3.0f;
	float LoadingTracker;
	
	float LoadingAndFadeRate;
	
	FTimerHandle BackgroundFadeTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BackgroundFadeTime = 0.7f;
	float BackgroundOpacity;
	float BackgroundAcceptanceFade;
	
	void StartBackgroundTransition();
	void FadeBackground();
	
	void LoadProgress();
	
public:
	void StartLoading();
	
};
