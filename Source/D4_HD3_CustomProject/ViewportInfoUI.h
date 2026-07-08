// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ViewportInfoUI.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UViewportInfoUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DisplayText;
	
	float AcceptanceDistance = 5.0f;
	float CurrentOpacity = 1;
	float FloatDisplacement = -300.0f;
	float FloatVelocity = -50.0f;
	float DisplacementTravelled = 0.0f;
	FVector2D CurrentLocation;
	FVector2D TargetLocation;
	
public:
	float GetFloatDistance() const;
	float GetAcceptanceDistance() const;
	
	void SetDisplayText(const FString& NewText) const;
	void SetFloatDistance(const float NewValue);
	void SetAcceptanceDistance(const float NewValue);
	
	void SetCurrentLocation(const FVector2D NewLocation);
};
