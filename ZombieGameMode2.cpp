// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieGameMode2.h"
#include "Blueprint/UserWidget.h"

void AZombieGameMode2::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr)
	{
		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
}