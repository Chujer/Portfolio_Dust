// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CEnemyCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Component/StateComponent.h"
#include "Component/WeaponComponent.h"
#include "Components/WidgetComponent.h"
#include "GameMode/CLobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widget/CGroggyWidget.h"
#include "Widget/CHPWidget.h"

ACEnemyCharacter::ACEnemyCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
	GroggyWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("GroggyWidget");
	GroggyWidgetComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	GroggyWidgetComponent->SetIsReplicated(true);
	GroggyWidgetComponent->SetRelativeLocation(FVector(0, 0, 70));
	GroggyWidgetComponent->SetRelativeRotation(FRotator(0, 0, 90));
	GroggyWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
}

void ACEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	ACLobbyGameMode* gameMode = Cast<ACLobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (gameMode != nullptr)
	{
		gameMode->OnLastPlayerInGame.AddDynamic(this, &ACEnemyCharacter::SetEnemyWeaponSet);
	}

	StateComponent->MakeHPUI();
	StateComponent->SetGroggyWidget(GroggyWidgetComponent->GetUserWidgetObject());
}

void ACEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


void ACEnemyCharacter::SetEnemyWeaponSet()
{
	WeaponComponent->SetWeaponData_Server(WeaponIndex);
}
