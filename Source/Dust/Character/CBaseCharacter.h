// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Misc/Structures.h"
#include "CBaseCharacter.generated.h"

UCLASS()
class DUST_API ACBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(Server, Reliable)
	void PlayMontage_Server(class UAnimMontage* AnimMontage = nullptr, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	UFUNCTION(NetMulticast, Reliable)
	void PlayMontage_NMC(class UAnimMontage* AnimMontage = nullptr, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UWeaponComponent> WeaponComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UStateComponent> StateComponent;


public:
	UPROPERTY(EditAnywhere)
	FHitAnimData HitData;
};
