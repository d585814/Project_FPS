// Fill out your copyright notice in the Description page of Project Settings.


#include "Enermy.h"
#include "Components/ArrowComponent.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimSequence.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/WidgetComponent.h"

// Sets default values
AEnermy::AEnermy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Health = 100.0f;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("hand_rSocket"));

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FireStartPoint"));
	Arrow->SetupAttachment(Weapon);
	
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	DeathImageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DeathImage"));
	DeathImageWidget->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("HeadSocket"));
	
}

// Called when the game starts or when spawned
void AEnermy::BeginPlay()
{
	Super::BeginPlay();

	DeathImageWidget->SetVisibility(false);
	
}




// Called to bind functionality to input
void AEnermy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnermy::Damaged(float Damage)
{
	Health -= Damage;
	if (Health < 1)
	{
		if (DeathAnimation != nullptr)
		{
			GetMesh()->PlayAnimation(DeathAnimation, false);
			GetCharacterMovement()->DisableMovement();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			DeathImageWidget->SetVisibility(true);
			bDeath = true;
		}

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
			{
				Destroy();
				GetWorld()->GetTimerManager().ClearTimer(Handle);
			}), 10.0f, false);
	}
}

void AEnermy::Fire()
{
	// NomalGun Enermy
	if (bNormalGun)
	{
		
		FVector Start = Arrow->GetComponentLocation();
		FVector End = Start + Arrow->GetForwardVector() * 10000.f;
		LineTrace(Start, End);

		//AutoMode
		FTimerHandle FireHandle;
		GetWorld()->GetTimerManager().SetTimer(FireHandle, FTimerDelegate::CreateLambda([&]()
			{
				Fire();
				GetWorld()->GetTimerManager().ClearTimer(FireHandle);
			}), 0.02f, false);
	}

	// Sniper Enermy
	else if (bSniper)
	{

		FVector Start = Arrow->GetComponentLocation();
		FVector End = Arrow->GetForwardVector() + UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation() +FVector(0.0f, 0.0f, 40.0f);
		FTimerHandle Handle;
		LineTrace(Start, End);
	}
}

void AEnermy::LineTrace(FVector StartPoint, FVector EndPoint)
{
	// Set LineTrace
	FHitResult HitResult;
	FVector Start = StartPoint;
	FVector End = EndPoint;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// Play GunAnimation 
	if (FireGunAnimation != nullptr)
	{
		Weapon->PlayAnimation(FireGunAnimation, false);
	}

	//LineTrace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, Params, FCollisionResponseParams()))
	{
		AMainCharacter* Character = Cast<AMainCharacter>(HitResult.GetActor());
		if (Character)
		{
			// Damage MainCharacter
			Character->Damaged(EnermyDamage);
			
			//Blood Effet
			if (BloodParticle != nullptr && CharacterHurtSound != nullptr)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodParticle, FVector(HitResult.ImpactPoint), FRotator(FMath::RandRange(-100.f, 100.f), 0.0f, 0.0f), FVector(0.5f));
				UGameplayStatics::PlaySound2D(GetWorld(), CharacterHurtSound);
			}
		}
	}
	
}

void AEnermy::SeeMainCharacter(AMainCharacter* Character)
{		
		Character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Character)
		{
			FVector Start = GetActorLocation();
			FVector Target = Character->GetActorLocation(); //+FVector(0.0f, -20.0f, -25.0f);
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);

			SetActorRotation(NewRotation);
			
		}
		
}
