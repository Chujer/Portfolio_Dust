// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CBaseCharacter.h"
#include "GameFramework/Character.h"
#include "CEnemyCharacter.generated.h"

UCLASS()
class DUST_API ACEnemyCharacter : public ACBaseCharacter
{
	GENERATED_BODY()

public:
	ACEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void SetEnemyWeaponSet();
	
public:
	UPROPERTY(EditAnywhere)
	int WeaponIndex = 0;
};

