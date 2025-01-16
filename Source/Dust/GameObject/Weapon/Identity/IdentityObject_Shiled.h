// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObject/Weapon/IdentityObject.h"
#include "IdentityObject_Shiled.generated.h"

/**
 * 
 */
UCLASS()
class DUST_API AIdentityObject_Shiled : public AIdentityObject
{
	GENERATED_BODY()

public:
	AIdentityObject_Shiled();

	virtual void BeginPlay() override;
	virtual void BeginIdentity() override;
	virtual void EndIdentity() override;

public:
	UPROPERTY(EditAnywhere)
	UAnimMontage* GuardAnim;

	class UStateComponent* StateComponent;


public:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
