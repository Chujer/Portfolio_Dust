// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/ExecuteTargetSubHP.h"

#include "Action/CDoAction.h"
#include "Character/CBaseCharacter.h"
#include "Component/StateComponent.h"
#include "Component/WeaponComponent.h"

void UExecuteTargetSubHP::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp->GetOwner()->HasAuthority())
		return;

	if (UWeaponComponent* weaponComponent = MeshComp->GetOwner()->GetComponentByClass<UWeaponComponent>())
	{
		if(UCDoAction* doAction = weaponComponent->GetDoAction())
		{
			if(doAction->GetExecuteTarget() != nullptr)
			{
				doAction->GetExecuteTarget()->StateComponent->SubHP(Damage);
			}
		}
	}
}
