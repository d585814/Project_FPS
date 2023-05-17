// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "AmmoPickup.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIE_API AAmmoPickup : public APickup
{
	GENERATED_BODY()
	
public:
	AAmmoPickup();

	virtual void OverlapEvent_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	bool bSniperAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	bool bRifleAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	bool bPistolAmmo;

	UFUNCTION(BlueprintPure , Category = "Ammo")
	float GetPlusAmmo() { return PlusAmmo; }

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void SetPlusAmmo(float Ammo) { PlusAmmo = Ammo; }


private:
	float PlusAmmo;
};
