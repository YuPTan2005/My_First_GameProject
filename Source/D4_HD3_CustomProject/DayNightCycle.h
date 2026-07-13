// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/SkyLight.h"
#include "DayNightCycle.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API ADayNightCycle : public AActor
{
	GENERATED_BODY()
	
public:
	ADayNightCycle();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Day Night Cycle")
	ADirectionalLight* SunDirectionalLight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ADirectionalLight* MoonDirectionalLight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Day Night Cycle")
	ASkyAtmosphere* SkyAtmosphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Day Night Cycle")
	AExponentialHeightFog* ExponentialHeightFog;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Day Night Cycle")
	ASkyLight* SkyLight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Day Night Cycle")
	AVolumetricCloud* VolumetricCloud;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Day Night Cycle")
	AStaticMeshActor* SkySphere;
	
	UPROPERTY()
	UExponentialHeightFogComponent* FogComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Day Night Cycle")
	UCurveFloat* FogDensity;
	
	float Time = 0.0f;
	float TimeDilation = 15.0f; // 360 seconds for one cycle
	float HoursPerDay = 24.0f;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Reset() override;
	
};
