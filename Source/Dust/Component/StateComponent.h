// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"

//캐릭터의 상태
UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle = 0, Hitted, Dead, Action, Roll, Max
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUST_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	void ChangeType(EStateType InType);

public:
	FORCEINLINE EStateType GetStateType() { return Type; }
	FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	FORCEINLINE bool isDeadMode() { return Type == EStateType::Dead; }
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsRollMode() { return Type == EStateType::Roll; }

public:
	void SetIdleMode();
	void SetHittedMode();
	void SetDeadMode();
	void SetActionMode();
	void SetRollMode();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type = EStateType::Idle;
};
