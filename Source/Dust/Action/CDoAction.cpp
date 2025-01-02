// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/CDoAction.h"

#include "CLog.h"
#include "Character/CPlayerCharacter.h"
#include "Component/MoveComponent.h"
#include "Component/StateComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
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
		//몽타주 재생중 캐릭터 이동불가 설정
		MoveComponent->SetStop(!DoActionDatas[ActionIndex].bCanMove);
	}
	if (OwnerCharacter.IsValid() && !!DoActionDatas[ActionIndex].Montage)
	{
		OwnerCharacter->PlayAnimMontage(DoActionDatas[ActionIndex].Montage, DoActionDatas[ActionIndex].PlayRate);
	}
}

void UCDoAction::EndDoAtion_Server()
{
	if (StateComponent == nullptr || MoveComponent == nullptr)
		return;

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

	//캐릭터 런치
	LaunchCharacter->LaunchCharacter(launchVector, true, true);
	//공격자도 함께 런치
	if (DoActionData.bWithLaunch)
		OwnerCharacter->LaunchCharacter(launchVector, true, true);
}

void UCDoAction::ApplyDamage(AActor* OtherActor, class AAttachment* Attachment)
{
	if (OtherActor == OwnerCharacter || Cast<ACharacter>(OtherActor) == nullptr)
		return;
	if (IsA(OwnerCharacter->GetClass())==false)
		return;

	CLog::Print(OtherActor->GetName());

	UGameplayStatics::ApplyDamage(OtherActor, DoActionDatas[ActionIndex].Power, OwnerCharacter->GetController(), 
		Cast<AActor>(Attachment), UDamageType::StaticClass());
}
