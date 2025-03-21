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
	void SetCustomTimeAndEnd(float MulTime, float EndTime);
	void ReturnCustomTIme();

public:    //시점 고정
	void LookAtTarget(const AActor* target);
	void LookAtLERP(const AActor* target);

public: 
	//사운드 출력 RPC
	UFUNCTION(Server,Reliable)
	void PlaySoundRPC(USoundBase* Sound);
	UFUNCTION(NetMulticast, Reliable)
	void PlaySoundRPC_NMC(USoundBase* Sound);

public:
	//Particle 실행
	UFUNCTION(Server, Reliable)
	void SpawnNiagaraRPC(UNiagaraSystem* Niagara, FVector Location, FRotator Rotator, FVector Scale);
	UFUNCTION(NetMulticast, Reliable)
	void SpawnNiagara_NMC(UNiagaraSystem* Niagara, FVector Location, FRotator Rotator, FVector Scale);
	

public:
	UFUNCTION(Server, Reliable)
	void PlayMontage_Server(class UAnimMontage* AnimMontage = nullptr, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	UFUNCTION(NetMulticast, Reliable)
	void PlayMontage_NMC(class UAnimMontage* AnimMontage = nullptr, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	UFUNCTION(Server, Reliable)
	void StopMontage_Server();

	UFUNCTION(NetMulticast, Reliable)
	void StopMontage_NMC();

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UWeaponComponent> WeaponComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UStateComponent> StateComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UFeetComponent> FeetComponent;

public:
	UPROPERTY(EditAnywhere, Category = "HitAnim")
	UAnimMontage* ParryHitAnimation;

	UPROPERTY(EditAnywhere, Category = "HitAnim")
	UAnimMontage* EndDownAnimation;

	UPROPERTY(EditAnywhere, Category = "HitAnim")
	UAnimMontage* HitNormalAnim;

	UPROPERTY(EditAnywhere, Category = "HitAnim")
	UAnimMontage* NockDownAnim;

private:
	FTimerHandle ReturnChangedCustomTimeTimerHandle;
};
