// Fill out your copyright notice in the Description page of Project Settings.


#include "DayNightCycle.h"

#include "Components/ExponentialHeightFogComponent.h"


ADayNightCycle::ADayNightCycle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADayNightCycle::BeginPlay()
{
	Super::BeginPlay();
	
	if (ExponentialHeightFog)
	{
		FogComponent = ExponentialHeightFog->GetComponent();
	}
}

void ADayNightCycle::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (SunDirectionalLight && MoonDirectionalLight && FogComponent)
	{
		if (Time >= HoursPerDay)
		{
			Time = 0.0f;
		}
		float LightRotation = Time / HoursPerDay * 360.0f;
		
		SunDirectionalLight->SetActorRelativeRotation(FRotator(LightRotation + 180.0f, 0, 0));
		MoonDirectionalLight->SetActorRelativeRotation(FRotator(LightRotation, 0, 0));
		
		if (FogDensity)
		{
			FogComponent->SetFogDensity(FogDensity->GetFloatValue(Time));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("FogDensity curve is not assigned in %s"), *GetName());
		}
		
		Time += DeltaSeconds / TimeDilation;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ExponentialHeightFog or Sun or Moon directional light is not assigned in %s"), *GetName());
	}
}

void ADayNightCycle::Reset()
{
	Super::Reset();
	
	Time = 0.0f;
}
