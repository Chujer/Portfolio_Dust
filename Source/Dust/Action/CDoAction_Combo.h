// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/CDoAction.h"
#include "CDoAction_Combo.generated.h"

/**
 * 
 */
UCLASS()
class DUST_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()
public:
	FORCEINLINE void SetComboTrigger() { ComboTrigger = true; }
	FORCEINLINE void SetAvailableTime(bool val) { AvailableTime = val; }

public:
	virtual void BeginPlay(ACharacter* InOwner) override;
	virtual void DoActionTrigger() override;
	virtual void DoAction_NMC() override;

	void NextDoAction();

private:
	TWeakObjectPtr<class UWeaponComponent> WeaponComponent;

	//콤보입력이 가능한 시간(노티파이 스테이트)
	bool AvailableTime = false;
	//추가공격 트리거
	bool ComboTrigger = false;
};
