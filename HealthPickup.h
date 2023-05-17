// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIE_API AHealthPickup : public APickup
{
	GENERATED_BODY()

public:
	AHealthPickup();

	virtual void OverlapEvent_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void SetAddHealth(float AddFloat){ AddHealth = AddFloat;}

private:
	float AddHealth;
	
};
