// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/CDoAction_Combo.h"

#include "Component/StateComponent.h"
#include "Component/WeaponComponent.h"
#include "GameFramework/Character.h"

void UCDoAction_Combo::BeginPlay(ACBaseCharacter* InOwner)
{
	Super::BeginPlay(InOwner);
	if (OwnerCharacter.IsValid())
		WeaponComponent = OwnerCharacter->GetComponentByClass<UWeaponComponent>();
}

void UCDoAction_Combo::DoActionTrigger()
{
	Super::DoActionTrigger();

	if (AvailableTime)
		ComboTrigger = true;
}

void UCDoAction_Combo::DoAction_NMC()
{
	Super::DoAction_NMC();
	ActionIndex++;
}

void UCDoAction_Combo::EndDoAtion_NMC()
{
	Super::EndDoAtion_NMC();
	ComboTrigger = false;
	AvailableTime = false;
}

void UCDoAction_Combo::NextDoAction()
{
	if (!ComboTrigger)
		return;

	StateComponent->SetIdleMode();

	ComboTrigger = false;
	AvailableTime = false;

	if(WeaponComponent.IsValid())
		WeaponComponent->DoAction_Server();
}
