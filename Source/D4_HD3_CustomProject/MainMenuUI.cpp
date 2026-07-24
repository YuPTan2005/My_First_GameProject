// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUI.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


void UMainMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	PlayButton->OnClicked.AddDynamic(this, &UMainMenuUI::OnPlayButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuUI::OnQuitButtonClicked);
}

void UMainMenuUI::OnPlayButtonClicked()
{
	UGameplayStatics::OpenLevel(this, FName("MainWorld"));
}

void UMainMenuUI::OnQuitButtonClicked()
{
	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* SpecificPlayer = World->GetFirstPlayerController();

	UKismetSystemLibrary::QuitGame(
		World,
		SpecificPlayer,
		EQuitPreference::Quit,
		false
	);
}
