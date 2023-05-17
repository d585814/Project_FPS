// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"

AHealthPickup::AHealthPickup()
{

}


void AHealthPickup::OverlapEvent_Implementation()
{
	Super::OverlapEvent_Implementation();
	AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		Player->PlusPlayerHealth(AddHealth);
		Destroy();
	}
}
