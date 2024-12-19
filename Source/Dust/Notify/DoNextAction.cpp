// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/DoNextAction.h"

#include "Action/CDoAction_Combo.h"
#include "Component/WeaponComponent.h"

void UDoNextAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(UWeaponComponent* WeaponComponent = MeshComp->GetOwner()->GetComponentByClass<UWeaponComponent>())
	{
		UCDoAction_Combo* ComboDoAction = Cast<UCDoAction_Combo>(WeaponComponent->GetDoAction());
		if(!!ComboDoAction)
		{
			ComboDoAction->NextDoAction();
		}
	}
}
