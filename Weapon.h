// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class ZOMBIE_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Accessories;

	UPROPERTY()
	class AMainCharacter* WeaponOwner;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Confiurations")
	FTransform EquipPlacement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Confiurations")
	FTransform IdlePlacement;
			
	UPROPERTY()
	bool bAutoMode;

	UPROPERTY()
	FTimerHandle TimerHandle;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentBulletNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TotalBulletNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultBulletNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch222")
	float Pitch;

	UPROPERTY()
	class AMainCharacter* Player;
				
	UFUNCTION()
	void Fire();
	

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetFireAnimation(UAnimationAsset* Animation) { FireAnimation = Animation;};

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetReloadAnimation(UAnimationAsset* Animation) { ReloadAnimation = Animation; };

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetBloodParticle(class UParticleSystem* Particle) {BloodParticle = Particle;};

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void SetHurtSound(class USoundBase* Sound2D);
	

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage;

	
	

private:
	UPROPERTY()
	UAnimationAsset* FireAnimation;

	UPROPERTY()
	UAnimationAsset* ReloadAnimation;

	UPROPERTY()
	class UParticleSystem* BloodParticle;

	UPROPERTY()
	class USoundBase* HurtSound;
};
