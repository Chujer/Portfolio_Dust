// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CEnemyCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Component/StateComponent.h"
#include "Component/WeaponComponent.h"
#include "GameMode/CLobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widget/CHPWidget.h"

ACEnemyCharacter::ACEnemyCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
	StateComponent = CreateDefaultSubobject<UStateComponent>("StateComponent");
	StateComponent->SetIsReplicated(true);
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
	WeaponComponent->SetIsReplicated(true);
}

void ACEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	ACLobbyGameMode* gameMode = Cast<ACLobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (gameMode != nullptr)
	{
		gameMode->OnLastPlayerInGame.AddDynamic(this, &ACEnemyCharacter::SetEnemyWeaponSet);
	}

	StateComponent->MakeBossUI();
}

void ACEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACEnemyCharacter::SetEnemyWeaponSet()
{
	WeaponComponent->SetWeaponData_Server(WeaponIndex);
}
