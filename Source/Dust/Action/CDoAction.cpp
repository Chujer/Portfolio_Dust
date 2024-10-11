// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/CDoAction.h"

#include "CLog.h"
#include "Component/MoveComponent.h"
#include "Component/StateComponent.h"
#include "GameFramework/Character.h"

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

void UCDoAction::DoAction_Implementation()
{
	StateComponent->SetActionMode();
	PlayMontage(DoActionDatas[Index]);
}


void UCDoAction::NextDoAction()
{
	PlayMontage(DoActionDatas[++Index]);
	if (Index >= MaxIndex)
		Index = 0;
}

void UCDoAction::EndDoAtion()
{
	StateComponent->SetIdleMode();
	MoveComponent->SetStop(false);
	Index = 0;
}

void UCDoAction::PlayMontage_Implementation(FDoActionData DoActionData)
{
	if (MoveComponent.IsValid())
	{
		//��Ÿ�� ����� ĳ���� �̵��Ұ� ����
		MoveComponent->SetStop(!DoActionData.bCanMove);
	}
	if (OwnerCharacter.IsValid() && !!DoActionData.Montage)
	{
		CLog::Print(DoActionData.Montage);
		OwnerCharacter->PlayAnimMontage(DoActionData.Montage, DoActionData.PlayRate);
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
