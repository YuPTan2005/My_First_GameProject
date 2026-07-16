// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "NPCInterface.h"
#include "NPCStatusComponent.h"
#include "GameFramework/Actor.h"
#include "DestructibleItem.generated.h"

UCLASS(Abstract)
class D4_HD3_CUSTOMPROJECT_API ADestructibleItem : public AActor, public IDamageable, public INPCInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructibleItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UNPCStatusComponent> HealthWidgetComponent;
	
	UPROPERTY(EditAnywhere, Category="Health")
	float MaxHealth = 100.0f;
	float CurrentHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time")
	float RestoreHealthSpeed = 0.15f;
	float RestoreTickRate = 0.25f;
	FTimerHandle HealthRestoreTimerHandle;
	void StartRestoreHealth();
	void RestoreHealth();
	
	bool bIsDead;
	bool bIsShowingUI;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time")
	float UIShowingTime = 15.0f;
	float ShowTimeTracker = 0.0f;
	float TimeTrackerRate = 1.0f;
	FTimerHandle ShowTimerHandle;
	void ResetShowingTime();
	void ReduceShowingTime();
	FTimerHandle ScaleWidgetTimer;
	float ScaleWidgetRate = 0.033f;
	void ScaleUIWidget();
	void ShowUI();
	void HideUI();
	void UpdateStatus() const;
	
	UFUNCTION(BlueprintCallable, Category = "UI Effects")
	void StartFade(bool bFadeIn);
	void UpdateFade();
	FTimerHandle FadeTimerHandle;
	bool bTargetFadeIn = false;
	float CurrentOpacity = 0.0f;
	const float FadeSpeed = 2.0f;
	const float FadeTickRate = 0.02f;
	
	virtual void OnDestructed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual float GetCurrentHealth_Implementation() override;
	virtual float GetMaxHealth_Implementation() override;
	
	virtual void DealDamage_Implementation(float DamageTaken, AActor* DamagedBy) override;
	virtual bool IsDead_Implementation() override;
	virtual EGameTeam GetTeam_Implementation() override;

};
