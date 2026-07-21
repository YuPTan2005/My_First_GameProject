// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem.h"
#include "PickupUI.h"
#include "GameFramework/Actor.h"
#include "PickupFood.generated.h"

UCLASS()
class D4_HD3_CUSTOMPROJECT_API APickupFood : public APickupItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupFood();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	UPROPERTY()
	TObjectPtr<UPickupUI> EatUI;
	UPROPERTY()
	TObjectPtr<UPickupUI> FeedUI;
	
	UPickupUI* CreateUIWidget(const FString& UIText, 
		const float UITextOffsetMultiplier,
		const FVector& VerticalOffset
		) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI properties")
	FString EatFoodText = "Press E to eat";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI properties")
	FString FeedFoodText = "Press F to feed";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI properties")
	float EatFoodTextRightOffset = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI properties")
	float FeedFoodTextRightOffset = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI properties")
	FVector EatFoodTextVerticalOffset = FVector(0, 0, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI properties")
	FVector FeedFoodTextVerticalOffset = FVector(0, 0, -50);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Collected(AActor* OtherActor);
	virtual void Collected_Implementation(AActor* OtherActor);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnCollected();
	virtual void UnCollected_Implementation();
	
	virtual AActor* PickedUp() override;

};
