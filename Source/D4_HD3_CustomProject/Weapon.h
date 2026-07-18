// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class D4_HD3_CUSTOMPROJECT_API AWeapon : public AActor, public IInventoryItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaponName = "DefaultWeaponName";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaponDescription = "DefaultWeaponDescription";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* WeaponImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SocketName = "WeaponSocket";

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual FString GetName_Implementation() override;
	virtual FString GetDescription_Implementation() override;
	UTexture2D* GetWeaponImage() const;
	FString GetSocketName();
	
	void SetWeaponMeshVisibility(const bool bNewVisibility) const;
};
