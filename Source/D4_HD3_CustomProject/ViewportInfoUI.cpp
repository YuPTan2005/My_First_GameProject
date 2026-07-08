// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewportInfoUI.h"

void UViewportInfoUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	TargetLocation = CurrentLocation + FVector2D(0, FloatDisplacement);
}

void UViewportInfoUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	DisplacementTravelled += InDeltaTime * FloatVelocity;
	const float DisplacementTravelledPercentage = DisplacementTravelled / FloatDisplacement;
	
	CurrentLocation = FMath::Lerp(
		CurrentLocation, 
		TargetLocation, 
		DisplacementTravelledPercentage);
	
	CurrentOpacity = FMath::Lerp(
		CurrentOpacity, 
		0, 
		DisplacementTravelledPercentage);
	
	DisplayText->SetOpacity(CurrentOpacity);
	SetPositionInViewport(CurrentLocation);
	if (FVector2D::Distance(CurrentLocation, TargetLocation) <= AcceptanceDistance)
	{
		RemoveFromParent();
	}
}

float UViewportInfoUI::GetFloatDistance() const
{
	return FloatDisplacement;
}

float UViewportInfoUI::GetAcceptanceDistance() const
{
	return AcceptanceDistance;
}

void UViewportInfoUI::SetFloatDistance(const float NewValue)
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

void UViewportInfoUI::SetCurrentLocation(const FVector2D NewLocation)
{
	CurrentLocation = NewLocation;
}
