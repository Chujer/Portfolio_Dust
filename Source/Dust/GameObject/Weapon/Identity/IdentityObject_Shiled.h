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
	virtual void BeginIdentitySkill() override;
	virtual void EndIdentitySkill() override;

public:
	UFUNCTION(Server, Reliable)
	void SetParry(bool isParry);
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void GuardOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void ParryOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere)
	UAnimMontage* GuardAnim;
	UPROPERTY(EditAnywhere)
	UAnimMontage* ParryingAnim;

	class UStateComponent* StateComponent;

private:
	UPROPERTY(Replicated)
	bool IsParrying = false;

public:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
