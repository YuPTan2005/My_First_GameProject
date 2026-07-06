// Fill out your copyright notice in the Description page of Project Settings.


#include "DayNightCycle.h"

#include "Kismet/GameplayStatics.h"


ADayNightCycle::ADayNightCycle()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ADayNightCycle::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(true);
	
	if (AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()))
	{
		SunLight = Cast<ADirectionalLight>(FoundActor);
	}
}

void ADayNightCycle::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (SunLight)
	{
		FRotator Rotation(DeltaSeconds * DaySpeed, 0.0f, 0.0f);
		SunLight->AddActorWorldRotation(Rotation);
	}
}
