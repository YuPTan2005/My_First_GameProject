// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewportInfoUI.h"

void UViewportInfoUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	TargetLocation = StartLocation + FVector2D(0, FloatDisplacement);
}

void UViewportInfoUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ElapsedTime += InDeltaTime;
	const float LerpAlpha = FMath::Clamp(ElapsedTime / TotalDuration, 0.0f, 1.0f);
	
	FVector2D CurrentLocation = FMath::Lerp(
		StartLocation, 
		TargetLocation, 
		LerpAlpha);
	
	float CurrentOpacity = FMath::Lerp(
		1.0f, 
		0.0f, 
		LerpAlpha);
	
	DisplayText->SetOpacity(CurrentOpacity);
	SetPositionInViewport(CurrentLocation);
	
	if (FVector2D::Distance(CurrentLocation, TargetLocation) <= AcceptanceDistance)
	{
		RemoveFromParent();
	}
}

float UViewportInfoUI::GetTotalDuration() const
{
	return TotalDuration;
}

void UViewportInfoUI::SetTotalDuration(const float NewValue)
{
	TotalDuration = NewValue;
}

float UViewportInfoUI::GetFloatDisplacement() const
{
	return FloatDisplacement;
}

float UViewportInfoUI::GetAcceptanceDistance() const
{
	return AcceptanceDistance;
}

void UViewportInfoUI::SetFloatDisplacement(const float NewValue)
{
	FloatDisplacement = NewValue;
}

void UViewportInfoUI::SetAcceptanceDistance(const float NewValue)
{
	AcceptanceDistance = NewValue;
}

void UViewportInfoUI::SetDisplayText(const FString& NewText) const
{
	DisplayText->SetText(FText::FromString(NewText));
}

void UViewportInfoUI::SetStartLocation(const FVector2D NewLocation)
{
	StartLocation = NewLocation;
}
