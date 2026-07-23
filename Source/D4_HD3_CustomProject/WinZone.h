// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerWonUI.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "WinZone.generated.h"

UCLASS()
class D4_HD3_CUSTOMPROJECT_API AWinZone : public AActor
{
	GENERATED_BODY()
	
public:	
	AWinZone();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UPlayerWonUI> WinUIClass;
	UPROPERTY()
	UPlayerWonUI* WinUIWidget;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						bool bFromSweep, const FHitResult& SweepResult);
	
	FTimerHandle ShowWonUITimer;
	void ShowWonUIWidget() const;
	
private:
	bool bHasTriggered = false;

};
