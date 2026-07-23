// Fill out your copyright notice in the Description page of Project Settings.


#include "WinZone.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWinZone::AWinZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetBoxExtent(FVector(200.0f, 200.0f, 100.0f));

}

// Called when the game starts or when spawned
void AWinZone::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AWinZone::OnOverlapBegin);
}

void AWinZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHasTriggered || !OtherActor)
	{
		return;
	}
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (OtherActor == PlayerPawn)
	{
		bHasTriggered = true;

		APlayerController* PC = Cast<APlayerController>(PlayerPawn->GetController());
		if (!PC)
		{
			return;
		}

		if (WinUIClass)
		{
			WinUIWidget = CreateWidget<UPlayerWonUI>(PC, WinUIClass);
			if (WinUIWidget)
			{
				WinUIWidget->AddToViewport();

				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(WinUIWidget->TakeWidget());
				PC->SetInputMode(InputMode);
				PC->bShowMouseCursor = true;
				
				GetWorldTimerManager().SetTimer(
					ShowWonUITimer,
					this,
					&AWinZone::ShowWonUIWidget,
					2.0f,
					false
					);
			}
		}
	}
}

void AWinZone::ShowWonUIWidget() const
{
	if (WinUIWidget && WinUIWidget->IsInViewport())
	{
		WinUIWidget->ShowUIWidget();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

