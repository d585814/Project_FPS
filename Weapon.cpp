// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Enermy.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"



// Sets default values
AWeapon::AWeapon()
{
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Root);
	Camera->bUsePawnControlRotation = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Camera);

	Accessories = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Accessories"));
	Accessories->SetupAttachment(Camera);

	Damage = 0.f;

	Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
}


// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (!WeaponOwner)
		Mesh->SetVisibility(true);
}


void AWeapon::Fire()
{
	if (CurrentBulletNum > 0)
	{
		CurrentBulletNum -= 1;
		FHitResult HitResult;
		FVector Start = Camera->GetComponentLocation();
		FVector End = Start + Camera->GetForwardVector() * 10000.f;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);


		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, Params, FCollisionResponseParams()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());

			AEnermy* Enermy = Cast<AEnermy>(HitResult.GetActor());
			if (Enermy)
			{
				
				Enermy->Damaged(Damage);
				

				// Blood Effect
				if (BloodParticle !=nullptr && HurtSound != nullptr)
				{
					 UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodParticle, FVector(HitResult.ImpactPoint),FRotator(FMath::RandRange(-100.f,100.f),0.0f,0.0f),FVector(0.5f));
					 UGameplayStatics::PlaySound2D(GetWorld(), HurtSound);
				}
				
			}
		}
		
		if (FireAnimation != nullptr)
		{
			Mesh->PlayAnimation(FireAnimation, false);
		}

		//Reload Weapon
		if (CurrentBulletNum < 1 && TotalBulletNum > 0)
		{		
			// Delay (0.01)
			FTimerHandle ReloadAnimTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(ReloadAnimTimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					// Set MainCharacter bReload(true)
					if (Player) { Player->bReload = true; }

					// Play WeaponReload Animaion
					if (ReloadAnimation != nullptr)
					{
						Mesh->PlayAnimation(ReloadAnimation, false);
					}
					GetWorld()->GetTimerManager().ClearTimer(ReloadAnimTimerHandle);
				}), 0.02f , false);


			// Delay (ReloadTime)
			FTimerHandle ReloadTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					//Set Bullet after Reload 
					CurrentBulletNum = DefaultBulletNum;
					TotalBulletNum = FMath::Clamp(TotalBulletNum -= DefaultBulletNum, 0.0f, 300.0f);

					//Set MainCharacter bReload(false)
					if (Player) { Player->bReload = false; }
					GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
				}), ReloadTime, false);	
			
		}



		// AutoMode (Only Rifle)
		if (bAutoMode)
		{			
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
				{

					Fire();
					
				}), 0.05f, false); 
		}

	}

}


void AWeapon::SetHurtSound(USoundBase* Sound2D)
{
	HurtSound = Sound2D;
}