// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/CDoAction.h"

#include "CLog.h"
#include "Component/MoveComponent.h"
#include "Component/StateComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
UCDoAction::UCDoAction()
{
}
void UCDoAction::BeginPlay(ACharacter* InOwner)
{
	OwnerCharacter = InOwner;
	if (OwnerCharacter.IsValid())
	{
		StateComponent = OwnerCharacter->GetComponentByClass<UStateComponent>();
		MoveComponent = OwnerCharacter->GetComponentByClass<UMoveComponent>();
	}
	MaxIndex = DoActionDatas.Num();
}

void UCDoAction::DoAction()
{
	StateComponent->SetActionMode();
}


void UCDoAction::NextDoAction()
{
	PlayMontage();
	Index++;
	if (Index >= MaxIndex)
		Index = 0;
}

void UCDoAction::EndDoAtion()
{
	StateComponent->SetIdleMode();
	MoveComponent->SetStop(false);
	Index = 0;
}

void UCDoAction::PlayMontage()
{
	if (OwnerCharacter.IsValid())
		CLog::Print(GetName());

	if (MoveComponent.IsValid())
	{
		//��Ÿ�� ����� ĳ���� �̵��Ұ� ����
		MoveComponent->SetStop(!DoActionDatas[Index].bCanMove);
	}
	if (OwnerCharacter.IsValid() && !!DoActionDatas[Index].Montage)
	{
		OwnerCharacter->PlayAnimMontage(DoActionDatas[Index].Montage, DoActionDatas[Index].PlayRate);
	}
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
