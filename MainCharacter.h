// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class ZOMBIE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* MinimapCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* CollectionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneCaptureComponent2D* MinimapScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<bool> WeaponMode;

	

	UPROPERTY()
	int32 OldIndex;

	UPROPERTY()
	TArray<class AWeapon*> Weapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AWeapon* CurrentWeapon;

	UPROPERTY()
	class AWeapon* OldWeapon;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSniperMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRifleMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPistolMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bKnifeMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bKnifeAttack;

	UPROPERTY()
	bool bRifleAutoMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bReload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bZoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOnLadder;

	


	UPROPERTY()
	FTimerHandle TimerHandle;

	
		
	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void JumpStart();

	UFUNCTION()
	void JumpEnd();

	UFUNCTION()
	void CrouchToggle();

	UFUNCTION()
	void RSide_Start();

	UFUNCTION()
	void RSide_End();

	UFUNCTION()
	void LSide_Start();

	UFUNCTION()
	void LSide_End();
	
	UFUNCTION(BlueprintCallable , Category="CallWeapon")
	void EquipWeapon1();

	UFUNCTION(BlueprintCallable, Category = "CallWeapon")
	void EquipWeapon2();

	UFUNCTION(BlueprintCallable, Category = "CallWeapon")
	void EquipWeapon3();

	UFUNCTION(BlueprintCallable, Category = "CallWeapon")
	void EquipWeapon4();

	UFUNCTION()
	void WeaponToggle(int32 index);

	UFUNCTION()
	void AttachWeapon(int32 WeaponIndex);

	UFUNCTION()
	void EquipOldWeapon(class AWeapon* Weapon);

	UFUNCTION()
	void FireOn();

	UFUNCTION()
	void FireOff();

	UFUNCTION(BlueprintNativeEvent,  Category= "Zoom")
	void ZoomOn();
	virtual void ZoomOn_Implementation();
		
	UFUNCTION(BlueprintNativeEvent, Category = "Zoom")
	void ZoomOff();
	virtual void ZoomOff_Implementation();

	UFUNCTION()
	void AutoModeToggle();
	
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetReloadAnimation(UAnimationAsset* Animation) { ReloadAnimation = Animation; };

	UFUNCTION(BlueprintPure)
	float GetWeaponCurrentBullet();

	UFUNCTION(BlueprintPure)
	float GetWeaponTotalBullet();

	UFUNCTION(BlueprintPure)
	float GetPlayerHealth() { return PlayerHealth;}

	UFUNCTION()
	void Damaged(float Damage);

	UFUNCTION()
	void DefaultZoomInSetting();

	UFUNCTION()
	void DefaultZoomOutSetting();

	UFUNCTION()
	void OverlapPickups();

	UFUNCTION()
	void PlusPlayerHealth(float AddHealth) { PlayerHealth += AddHealth;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// ½º³ªÀÌÆÛ ÁÜ À§Á¬Å¬¶ó½º
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> SniperZoomWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class AWeapon>> DefaultWeapons;

	UPROPERTY()
	class UUserWidget* SniperWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> DeathWidgetClass;

	UPROPERTY()
	class UUserWidget* DeathWidget;

	UPROPERTY()
	UAnimationAsset* ReloadAnimation;

private:
	
	float PlayerHealth;
	

};
