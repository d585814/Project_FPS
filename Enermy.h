// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enermy.generated.h"

UCLASS()
class ZOMBIE_API AEnermy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnermy();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void Damaged(float Damage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* Arrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* DeathImageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnermyDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNormalGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSniper;
		
	UFUNCTION(BlueprintCallable, Category= "Enermy Fire")
	void Fire();
	
	UFUNCTION()
	void LineTrace(FVector StartPoint, FVector EndPoint);

	UFUNCTION(BlueprintCallable, Category = "Sensing Chaaracter")
	void SeeMainCharacter(class AMainCharacter* Character);
	
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetFireGunAnimation(UAnimSequence* AnimationSequence) { FireGunAnimation = AnimationSequence;};

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetDeathAnimation(UAnimSequence* AnimationSequence) { DeathAnimation = AnimationSequence; };

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetBloodParticle(class UParticleSystem* Particle) { BloodParticle = Particle; };
	
	UFUNCTION(BlueprintPure, Category = "Death")
	bool GetWasDeath() { return bDeath;}

	UFUNCTION(BlueprintCallable, Category = "Sound")
		void SetCharacterHurtSound(class USoundBase* Sound2D) {	CharacterHurtSound = Sound2D;};


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	float Health;

	UPROPERTY()
	class UAnimSequence* FireGunAnimation;

	UPROPERTY()
	class UAnimSequence* DeathAnimation;

	UPROPERTY()
	bool bDeath;

	UPROPERTY()
	class UParticleSystem* BloodParticle;

	UPROPERTY()
	class USoundBase* CharacterHurtSound;

};
