// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StateComponent.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Widget/CHPWidget.h"

UStateComponent::UStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UStateComponent, Type);
	DOREPLIFETIME(UStateComponent, HP);
	DOREPLIFETIME(UStateComponent, MaxHP);
}


void UStateComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (OwnerCharacter->GetController() == nullptr)
		return;

		HP = MaxHP;

}

void UStateComponent::MakeBossUI()
{

	HPWidget = Cast<UCHPWidget>(CreateWidget(GetWorld(), HPWidgetClass));
	HPWidget->AddToViewport();
	HPWidget->MaxHP = MaxHP;
	HPWidget->HP = HP;
}

void UStateComponent::ChangeType_Implementation(EStateType InType)
{
	EStateType prevType = Type;
	Type = InType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, Type);
}

void UStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UStateComponent::SetRollMode()
{
	ChangeType(EStateType::Roll);
}

void UStateComponent::SubHP(float Damage)
{
	HP -= Damage;
	if (HP <= 0)
	{
		SetDeadMode();
	}
}


void UStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (HPWidget == nullptr)
		return;
	HPWidget->HP = HP;
}

