// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AvailableComboTime.h"

#include "Action/CDoAction_Combo.h"
#include "Component/WeaponComponent.h"

void UAvailableComboTime::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (UWeaponComponent* weaponComponent = MeshComp->GetOwner()->GetComponentByClass<UWeaponComponent>())
	{
		ComboDoAction = Cast<UCDoAction_Combo>(weaponComponent->GetDoAction());
		if (ComboDoAction.IsValid())
			ComboDoAction->SetAvailableTime(true);
	}
}

void UAvailableComboTime::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ComboDoAction.IsValid())
		ComboDoAction->SetAvailableTime(false);
}
