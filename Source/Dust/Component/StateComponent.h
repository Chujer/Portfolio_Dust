// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"

//캐릭터의 상태
UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle = 0, Hitted, Dead, Action, Roll, Groggy, HittingParry, Max
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUST_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStateComponent();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(NetMulticast, Reliable)
	void PlayAnimMontage_NMC(UAnimMontage* montage);
public:
	void MakeHPUI();

public:
	UFUNCTION(Server, Reliable)
	void ChangeType(EStateType InType);

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EStateType GetStateType() { return Type; }
	FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	FORCEINLINE bool isDeadMode() { return Type == EStateType::Dead; }
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsRollMode() { return Type == EStateType::Roll; }
	FORCEINLINE bool IsGroggyMode() { return Type == EStateType::Groggy; }
	FORCEINLINE bool IsHittingParryMode() { return Type == EStateType::HittingParry; }

public:
	void SetIdleMode();
	void SetHittedMode();
	void SetDeadMode();
	void SetActionMode();
	void SetRollMode();
	void SetGroggyMode();
	void SetHittingParryMode();

public:
	void SubHP(float Damage);

public:
	UFUNCTION()
	void OnHittingParry(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
	void OnGroggy(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
	void EndGroggy();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FStateTypeChanged OnStateTypeChanged;


private:
	UPROPERTY(EditAnywhere, Replicated)
	EStateType Type = EStateType::Idle;

	TWeakObjectPtr<class ACBaseCharacter> OwnerCharacter;

private:
	FTimerHandle Timer;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCHPWidget> HPWidgetClass;

	class UCHPWidget* HPWidget;


	UPROPERTY(EditAnywhere, Replicated)
	float MaxHP = 100;
	UPROPERTY(EditAnywhere, Replicated)
	float HP = 100;
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	float GroggyTime = 0;
};
