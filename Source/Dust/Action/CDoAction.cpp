// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/CDoAction.h"

#include "CLog.h"
#include "Component/MoveComponent.h"
#include "Component/StateComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
UCDoAction::UCDoAction()
{
	MaxActionIndex = DoActionDatas.Num();
}
void UCDoAction::BeginPlay(ACharacter* InOwner)
{
	OwnerCharacter = InOwner;
	if (OwnerCharacter.IsValid())
	{
		StateComponent = OwnerCharacter->GetComponentByClass<UStateComponent>();
		MoveComponent = OwnerCharacter->GetComponentByClass<UMoveComponent>();
	}
}

void UCDoAction::DoActionTrigger()
{
}

void UCDoAction::DoAction_Server()
{
	if (!StateComponent.IsValid())
		return;

	StateComponent->SetActionMode();
}

void UCDoAction::DoAction_NMC()
{
	if (MoveComponent.IsValid())
	{
		//��Ÿ�� ����� ĳ���� �̵��Ұ� ����
		MoveComponent->SetStop(!DoActionDatas[ActionIndex].bCanMove);
	}
	if (OwnerCharacter.IsValid() && !!DoActionDatas[ActionIndex].Montage)
	{
		OwnerCharacter->PlayAnimMontage(DoActionDatas[ActionIndex].Montage, DoActionDatas[ActionIndex].PlayRate);
	}
}

void UCDoAction::EndDoAtion_Server()
{
	StateComponent->SetIdleMode();
	MoveComponent->SetStop(false);
}

void UCDoAction::EndDoAtion_NMC()
{
	ActionIndex = 0;
}

void UCDoAction::LaunchCharacter(FDoActionData DoActionData, ACharacter* LaunchCharacter)
{
	if (!OwnerCharacter.IsValid() || !LaunchCharacter)
		return;

	FVector launchVector = OwnerCharacter->GetActorForwardVector() * DoActionData.Launch.X;
	launchVector.Z = DoActionData.Launch.Z;

	//ĳ���� ��ġ
	LaunchCharacter->LaunchCharacter(launchVector, true, true);
	//�����ڵ� �Բ� ��ġ
	if (DoActionData.bWithLaunch)
		OwnerCharacter->LaunchCharacter(launchVector, true, true);
}
