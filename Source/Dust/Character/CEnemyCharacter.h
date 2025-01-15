// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CEnemyCharacter.generated.h"

UCLASS()
class DUST_API ACEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void SetEnemyWeaponSet();

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UWeaponComponent> WeaponComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UStateComponent> StateComponent;

public:
	UPROPERTY(EditAnywhere)
	int WeaponIndex = 0;
};

