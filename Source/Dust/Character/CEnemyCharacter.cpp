// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CEnemyCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Component/StateComponent.h"
#include "Component/WeaponComponent.h"
#include "GameMode/CLobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widget/CBossHPWidget.h"

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
	MakeBossUI();


	ACLobbyGameMode* gameMode = Cast<ACLobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (gameMode != nullptr)
	{
		gameMode->OnLastPlayerInGame.AddDynamic(this, &ACEnemyCharacter::SetEnemyWeaponSet);
	}
}

void ACEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HPWidget->HP = StateComponent->HP;

}

void ACEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void ACEnemyCharacter::MakeBossUI()
{
	HPWidget = Cast<UCBossHPWidget>(CreateWidget(GetWorld(), HPWidgetClass));
	HPWidget->AddToViewport();
	HPWidget->MaxHP = StateComponent->MaxHP;
	HPWidget->HP = StateComponent->HP;
}

void ACEnemyCharacter::SetEnemyWeaponSet()
{
	WeaponComponent->SetWeaponData_Server(WeaponIndex);
}


//void ACEnemyCharacter::MakeBossUI_Implementation()
//{
//	HPWidget = CreateWidget(GetWorld(), HPWidgetClass);
//	HPWidget->AddToViewport();
//}

