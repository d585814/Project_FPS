// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Math/RotationMatrix.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Weapon.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Pickup.h"
#include "Components/SphereComponent.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	MinimapCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MinimapCamera"));
	MinimapCamera ->SetupAttachment(RootComponent);

	MinimapScene = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapScene"));
	MinimapScene->SetupAttachment(MinimapCamera);

	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(200.f);

	WeaponMode.Add(false);
	WeaponMode.Add(false);
	WeaponMode.Add(false);
	WeaponMode.Add(false);

	PlayerHealth = 100.f;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	for (const TSubclassOf<AWeapon>& WeaponClass : DefaultWeapons)
	{
		if (!WeaponClass)continue;
		FActorSpawnParameters Params;
		int32 Index = Weapons.Add(GetWorld()->SpawnActor<AWeapon>(WeaponClass, Params));
		FTransform SpawnTransform = Weapons[Index]->IdlePlacement * GetMesh()->GetSocketTransform(FName("EquipSocket"));
		Weapons[Index]->SetActorTransform(SpawnTransform, false, nullptr, ETeleportType::TeleportPhysics);
		Weapons[Index]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("EquipSocket"));
	}
		
}

// Tick Event
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentWeapon != nullptr && !bKnifeMode)
	{
		if (bZoom)
		{
			// CurrentWeapon Camera LookUp Control in ZoomMode 
			CurrentWeapon->Pitch = FMath::Clamp(CurrentWeapon->Pitch + InputComponent->GetAxisValue(FName("LookUp")) + DeltaTime, -60.f, 60.f);
			CurrentWeapon->Camera->SetRelativeRotation(FRotator(CurrentWeapon->Pitch * -1.0f, 0.0f, 0.0f));
		}
	}

	
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::JumpStart);
	InputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::JumpEnd);
	InputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::CrouchToggle);

	InputComponent->BindAction("Weapon1", IE_Pressed, this, &AMainCharacter::EquipWeapon1);
	InputComponent->BindAction("Weapon2", IE_Pressed, this, &AMainCharacter::EquipWeapon2);
	InputComponent->BindAction("Weapon3", IE_Pressed, this, &AMainCharacter::EquipWeapon3);
	InputComponent->BindAction("Weapon4", IE_Pressed, this, &AMainCharacter::EquipWeapon4);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::FireOn);
	InputComponent->BindAction("Fire", IE_Released, this, &AMainCharacter::FireOff);

	InputComponent->BindAction("GetItem", IE_Pressed, this, &AMainCharacter::OverlapPickups);
	
	InputComponent->BindAction("AutoMode", IE_Pressed, this, &AMainCharacter::AutoModeToggle);

	InputComponent->BindAction("Rifle_RSide", IE_Pressed, this, &AMainCharacter::RSide_Start);
	InputComponent->BindAction("Rifle_RSide", IE_Released, this, &AMainCharacter::RSide_End);
	InputComponent->BindAction("Rifle_LSide", IE_Pressed, this, &AMainCharacter::LSide_Start);
	InputComponent->BindAction("Rifle_LSide", IE_Released, this, &AMainCharacter::LSide_End);


	InputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	InputComponent->BindAxis("LookUp", this, &AMainCharacter::AddControllerPitchInput);
	InputComponent->BindAxis("Turn", this, &AMainCharacter::AddControllerYawInput);
			
	
}

// ----------------------------------------------------Input Function-------------------------------------------------------------------

void AMainCharacter::MoveForward(float Value)
{
	if (!bOnLadder)
	{
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		AddMovementInput(GetActorUpVector(), Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if (!bOnLadder)
	{
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::JumpStart()
{
	bPressedJump = true;
}

void AMainCharacter::JumpEnd()
{
	bPressedJump = false;
}

void AMainCharacter::CrouchToggle()
{
	if (!bCrouch)
	{
		bCrouch = true;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
	else
	{
		bCrouch = false;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}

void AMainCharacter::RSide_Start()
{
	bRSide = true; 
}

void AMainCharacter::RSide_End()
{
	bRSide = false;
}

void AMainCharacter::LSide_Start()
{
	bLSide = true;
}

void AMainCharacter::LSide_End()
{
	bLSide = false;
}
// ----------------------------------------------Input Function End------------------------------------------------------------//





void AMainCharacter::EquipWeapon1()
{		
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
		{
			int32 index = 0;
			WeaponToggle(index);
			GetWorld()->GetTimerManager().ClearTimer(Handle);
		}), 0.1f, false);
}

void AMainCharacter::EquipWeapon2()
{	
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
		{
			int32 index = 1;
			WeaponToggle(index);
			GetWorld()->GetTimerManager().ClearTimer(Handle);
		}), 0.1f, false);
	
}

void AMainCharacter::EquipWeapon3()
{
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
		{
			int32 index = 2;
			WeaponToggle(index);
			GetWorld()->GetTimerManager().ClearTimer(Handle);
		}), 0.1f, false);
}

void AMainCharacter::EquipWeapon4()
{
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
		{
			int32 index = 3;
			WeaponToggle(index);
			GetWorld()->GetTimerManager().ClearTimer(Handle);
		}), 0.1f, false);
		
}


void AMainCharacter::WeaponToggle(int32 index)
{
	
	if (Weapons[index])
	{
		
		if (WeaponMode[OldIndex])
		{

			if (index == OldIndex)
			{
				WeaponMode[index] = false;
				EquipOldWeapon(OldWeapon);
				CurrentWeapon = nullptr;
			}
			else
			{
				
				WeaponMode[OldIndex] = false;
				EquipOldWeapon(OldWeapon);
				WeaponMode[index] = true;
				AttachWeapon(index);
			}


		}
		else if (!WeaponMode[index])
		{			
			WeaponMode[index] = true;
			AttachWeapon(index);
		}
				
		bSniperMode = WeaponMode[0];
		bRifleMode = WeaponMode[1];
		bPistolMode = WeaponMode[2];
		bKnifeMode = WeaponMode[3];
	}	
}


void AMainCharacter::AttachWeapon(int32 WeaponIndex)
{
	CurrentWeapon = Weapons[WeaponIndex];
	if (CurrentWeapon)
	{
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
			{
				FTransform SpawnTransform = CurrentWeapon->EquipPlacement * GetMesh()->GetSocketTransform(FName("hand_rSocket"));
				CurrentWeapon->SetActorTransform(SpawnTransform, false, nullptr, ETeleportType::TeleportPhysics);
				CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("hand_rSocket"));
				GetWorld()->GetTimerManager().ClearTimer(Handle);
			}), 0.6f, false);
		

		

	}
	OldWeapon = CurrentWeapon;
	OldIndex = WeaponIndex;	
}

void AMainCharacter::EquipOldWeapon(AWeapon* Weapon)
{			
	FTransform SpawnTransform = Weapon->IdlePlacement * GetMesh()->GetSocketTransform(FName("EquipSocket"));
	Weapon->SetActorTransform(SpawnTransform, false, nullptr, ETeleportType::TeleportPhysics);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("EquipSocket"));	
	
}


void AMainCharacter::FireOn()
{
	if (CurrentWeapon)
	{					
		bKnifeAttack = true;
		if (CurrentWeapon->CurrentBulletNum > 0)
		{
				
			if (!bKnifeMode)
			{
				if (bSniperMode)
				{
					CurrentWeapon->Fire();
							
				}
				else { CurrentWeapon->Fire(); }
					
			}

		}
		
	}
}


void AMainCharacter::FireOff()
{
	
	if (CurrentWeapon)
	{
		bKnifeAttack = false;
		GetWorld()->GetTimerManager().ClearTimer(CurrentWeapon->TimerHandle);
	}
	
}

void AMainCharacter::ZoomOn_Implementation()
{
	if (bSniperMode)
	{
		DefaultZoomInSetting();
		CurrentWeapon->Mesh->SetOwnerNoSee(true);
		GetMesh()->bPauseAnims=true;
		
		if (SniperZoomWidgetClass != nullptr)
		{
			SniperWidget = CreateWidget<UUserWidget>(GetWorld(), SniperZoomWidgetClass);
			if (SniperWidget != nullptr)
			{
				SniperWidget -> AddToViewport();
			}
		}
		
	}

	else if(bRifleMode || bPistolMode)
	{
		DefaultZoomInSetting();		
	}
}

void AMainCharacter::ZoomOff_Implementation()
{	
	if (bSniperMode)
	{
		DefaultZoomOutSetting();
		CurrentWeapon->Mesh->SetOwnerNoSee(false);
		GetMesh()->bPauseAnims = false;
		if (SniperWidget != nullptr)
		{
			SniperWidget->RemoveFromParent();
		}
	}

	else if(bRifleMode || bPistolMode)
	{
		DefaultZoomOutSetting();
		
	}
}

void AMainCharacter::AutoModeToggle()
{
	if (bRifleMode)
	{
		if (!CurrentWeapon->bAutoMode)
		{
			CurrentWeapon->bAutoMode = true;
			
		}
		else
		{
			CurrentWeapon->bAutoMode = false;
			
		}
	}
}

float AMainCharacter::GetWeaponCurrentBullet()
{
	if (CurrentWeapon!=nullptr)
	{
		return CurrentWeapon->CurrentBulletNum;
	}
	else
	{
		return 0.f;
	}
}

float AMainCharacter::GetWeaponTotalBullet()
{
	if (CurrentWeapon != nullptr)
	{
		return CurrentWeapon->TotalBulletNum;
	}
	else
	{
		return 0.f;
	}
}

void AMainCharacter::Damaged(float Damage)
{
	PlayerHealth -= Damage;
	if (PlayerHealth < 1 )
	{
		//Death Event
		//Death Widget Valid Check
		if (DeathWidgetClass!= nullptr)
		{
			// Create DeathWidget
			DeathWidget = CreateWidget<UUserWidget>(GetWorld(), DeathWidgetClass);
			DeathWidget->AddToViewport();
			// Game Paused
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			// Show Mouse Cursor
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
		}
	}
}

void AMainCharacter::DefaultZoomInSetting()
{
	bZoom = true;
	EViewTargetBlendFunction BlendFuc;
	UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(CurrentWeapon, 0.f, BlendFuc = VTBlend_Linear, 0.f, false);
	CurrentWeapon->EnableInput(UGameplayStatics::GetPlayerController(this, 0));
	
}

void AMainCharacter::DefaultZoomOutSetting()
{
	bZoom = false;
	EViewTargetBlendFunction BlendFuc;
	UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(this, 0.f, BlendFuc = VTBlend_Linear, 0.f, false);
	CurrentWeapon->DisableInput(UGameplayStatics::GetPlayerController(this, 0));
	CurrentWeapon->Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
}

void AMainCharacter::OverlapPickups()
{
	TArray<AActor*>CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	//keep track of the collected battery power
	float CollectedPower = 0;

	// For each Actor we Collected
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); iCollected++)
	{
		// Cast the actor to APickup
		APickup* const MyPickup = Cast<APickup>(CollectedActors[iCollected]);

		// If the cast is successful and the pickup is valid and active
		if (MyPickup && !MyPickup->IsPendingKill())
		{
			// call the pickup's WasCollected function
			MyPickup->OverlapEvent();
		}
	}
}