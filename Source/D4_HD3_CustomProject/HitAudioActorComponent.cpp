// Fill out your copyright notice in the Description page of Project Settings.


#include "HitAudioActorComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHitAudioActorComponent::UHitAudioActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

void UHitAudioActorComponent::PlayHitSound(const FString& HitBy) const
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	USoundBase* SoundToPlay = nullptr;
	
	if (HitBy.Equals(TEXT("Fist"), ESearchCase::IgnoreCase))
	{
		SoundToPlay = PunchHitAudio;
	}
	else if (HitBy.Equals(TEXT("Sword"), ESearchCase::IgnoreCase))
	{
		SoundToPlay = SwordHitAudio;
	}
	else if (HitBy.Equals(TEXT("Hammer"), ESearchCase::IgnoreCase))
	{
		SoundToPlay = HammerHitAudio;
	}
	
	if (SoundToPlay)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			SoundToPlay,
			Owner->GetActorLocation()
		);
	}
}
