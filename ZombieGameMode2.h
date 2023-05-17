// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZombieGameMode2.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIE_API AZombieGameMode2 : public AGameModeBase
{
	GENERATED_BODY()

public:


protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget", meta = (BlueprintProtected = "true"))
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY()
	class UUserWidget* HUDWidget;
	
};
