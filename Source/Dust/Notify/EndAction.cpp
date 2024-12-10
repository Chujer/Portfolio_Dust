// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/EndAction.h"

#include "Character/CPlayerCharacter.h"
#include "Component/StateComponent.h"
#include "Component/WeaponComponent.h"


void UEndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (UWeaponComponent* weaponComponent = MeshComp->GetOwner()->GetComponentByClass<UWeaponComponent>())
	{
		weaponComponent->EndDoAction_Server();
	}
}
