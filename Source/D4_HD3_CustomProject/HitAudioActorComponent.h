// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitAudioActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class D4_HD3_CUSTOMPROJECT_API UHitAudioActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHitAudioActorComponent();
	
	void PlayHitSound(const FString& HitBy) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio")
	USoundBase* PunchHitAudio;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio")
	USoundBase* SwordHitAudio;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio")
	USoundBase* HammerHitAudio;
		
};
