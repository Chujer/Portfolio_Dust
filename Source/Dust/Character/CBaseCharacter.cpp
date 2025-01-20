// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CBaseCharacter.h"

#include "Component/StateComponent.h"
#include "Component/WeaponComponent.h"

// Sets default values
ACBaseCharacter::ACBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StateComponent = CreateDefaultSubobject<UStateComponent>("StateComponent");
	StateComponent->SetIsReplicated(true);
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
	WeaponComponent->SetIsReplicated(true);
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

void ACBaseCharacter::PlayMontage_NMC_Implementation(UAnimMontage* AnimMontage, float InPlayRate,
	FName StartSectionName)
{
	PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);
}

void ACBaseCharacter::PlayMontage_Server_Implementation(UAnimMontage* AnimMontage, float InPlayRate,
                                                        FName StartSectionName)
{
	if (AnimMontage == nullptr)
		return;

	PlayMontage_NMC(AnimMontage, InPlayRate, StartSectionName);
}

