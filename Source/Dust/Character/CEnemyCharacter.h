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

	//UFUNCTION(NetMulticast, Reliable)
	void MakeBossUI();

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UStateComponent> StateComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCBossHPWidget> HPWidgetClass;
	class UCBossHPWidget* HPWidget;
};

