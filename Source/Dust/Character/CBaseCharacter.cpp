// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CBaseCharacter.h"

#include "NiagaraFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Component/FeetComponent.h"
#include "Component/StateComponent.h"
#include "Component/WeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACBaseCharacter::ACBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StateComponent = CreateDefaultSubobject<UStateComponent>("StateComponent");
	StateComponent->SetIsReplicated(true);
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
	WeaponComponent->SetIsReplicated(true);
	FeetComponent = CreateDefaultSubobject<UFeetComponent>("FeetComponent");
	FeetComponent->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void ACBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACBaseCharacter::SetCustomTimeAndEnd(float MulTime, float EndTime)
{
	CustomTimeDilation = MulTime;
	GetWorldTimerManager().SetTimer(ReturnChangedCustomTimeTimerHandle, this, &ACBaseCharacter::ReturnCustomTIme, EndTime, false, EndTime);
}

void ACBaseCharacter::ReturnCustomTIme()
{
	CustomTimeDilation = 1.0f;
	GetWorldTimerManager().ClearTimer(ReturnChangedCustomTimeTimerHandle);
}

void ACBaseCharacter::LookAtTarget(const AActor* target)
{
	if (target == nullptr)
		return;


	FRotator rotate = GetActorRotation();
	rotate.Yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target->GetActorLocation()).Yaw;
	
	SetActorRotation(rotate);
}

void ACBaseCharacter::LookAtLERP(const AActor* target)
{
	if (target == nullptr)
		return;
	
	FRotator currentRotation = GetActorRotation();
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target->GetActorLocation());

	float deltaYaw = UKismetMathLibrary::NormalizeAxis(targetRotation.Yaw - currentRotation.Yaw);

	if (deltaYaw > 180.0f)
		deltaYaw -= 360.0f;
	else if (deltaYaw < -180.0f)
		deltaYaw += 360.0f;

	float newYaw = FMath::FInterpTo(currentRotation.Yaw, currentRotation.Yaw + deltaYaw, GetWorld()->GetDeltaSeconds(), 10.0f);

	
    FRotator newRotation = FRotator(currentRotation.Pitch, newYaw, currentRotation.Roll);
    SetActorRotation(newRotation);
}

void ACBaseCharacter::SpawnNiagaraRPC_Implementation(UNiagaraSystem* Niagara, FVector Location, FRotator Rotator, FVector Scale)
{
	SpawnNiagara_NMC(Niagara, Location, Rotator, Scale);
}

void ACBaseCharacter::SpawnNiagara_NMC_Implementation(UNiagaraSystem* Niagara, FVector Location, FRotator Rotator, FVector Scale)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Niagara, Location, Rotator, Scale);
}

void ACBaseCharacter::PlaySoundRPC_Implementation(USoundBase* Sound)
{
	PlaySoundRPC_NMC(Sound);
}

void ACBaseCharacter::PlaySoundRPC_NMC_Implementation(USoundBase* Sound)
{
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
}

void ACBaseCharacter::StopMontage_Server_Implementation()
{
	StopMontage_NMC();
}

void ACBaseCharacter::StopMontage_NMC_Implementation()
{
	StopAnimMontage();
}

void ACBaseCharacter::PlayMontage_NMC_Implementation(UAnimMontage* AnimMontage, float InPlayRate,
                                                     FName StartSectionName)
{
	if (AnimMontage == nullptr)
		StopAnimMontage();
	else
		PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);
}

void ACBaseCharacter::PlayMontage_Server_Implementation(UAnimMontage* AnimMontage, float InPlayRate,
                                                        FName StartSectionName)
{
	PlayMontage_NMC(AnimMontage, InPlayRate, StartSectionName);
}

