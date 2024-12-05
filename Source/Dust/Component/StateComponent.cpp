// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StateComponent.h"

#include "Net/UnrealNetwork.h"

UStateComponent::UStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UStateComponent, Type);
}


void UStateComponent::BeginPlay()
{
	Super::BeginPlay();
	
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


void UStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

