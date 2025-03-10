// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AttachmentCollision.h"

#include "Component/WeaponComponent.h"
#include "GameObject/Weapon/Attachment.h"

void UAttachmentCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (UWeaponComponent* weaponComponent = MeshComp->GetOwner()->GetComponentByClass<UWeaponComponent>())
	{
		if (weaponComponent->GetAttachment() != nullptr)
			weaponComponent->GetAttachment()->SetCollision(ECollisionEnabled::QueryOnly);

		if (weaponComponent->GetAttachment() != nullptr)
			weaponComponent->GetAttachment()->isNormalDamage = IsNormalDamage;
	}
}

void UAttachmentCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (UWeaponComponent* weaponComponent = MeshComp->GetOwner()->GetComponentByClass<UWeaponComponent>())
	{
		if (weaponComponent->GetAttachment() != nullptr)
		{
			weaponComponent->GetAttachment()->SetCollision(ECollisionEnabled::NoCollision);
			weaponComponent->GetAttachment()->ClearHittedCharacter();
		}
	}
}
