// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PickupUI.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UPickupUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DisplayText;
	
	virtual void NativeConstruct() override;
	
public:
	FVector2D CurrentLocation;
	
	void SetDisplayText(const FString& NewText) const;
	
};
