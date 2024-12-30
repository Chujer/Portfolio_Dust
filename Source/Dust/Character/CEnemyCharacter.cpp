// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CEnemyCharacter.h"

ACEnemyCharacter::ACEnemyCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

	
}

void ACEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

