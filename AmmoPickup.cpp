// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon.h"

AAmmoPickup::AAmmoPickup()
{

}

void AAmmoPickup::OverlapEvent_Implementation()
{
	Super::OverlapEvent_Implementation();

	AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		//Sniper Ammo Plus
		if (bSniperAmmo)
		{
			Player->Weapons[0]->TotalBulletNum += PlusAmmo;
			Destroy();
		}
		//Rifle Ammo Plus
		else if (bRifleAmmo)
		{
			Player->Weapons[1]->TotalBulletNum += PlusAmmo;
			Destroy();
		}
		//Sniper Ammo Plus
		else if (bPistolAmmo)
		{
			Player->Weapons[2]->TotalBulletNum += PlusAmmo;
			Destroy();
		}

	}
}