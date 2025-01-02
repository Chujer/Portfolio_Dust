// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AttachmentCollision.h"

#include "Component/WeaponComponent.h"
#include "GameObject/Weapon/Attachment.h"

void UAttachmentCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);


	if (UWeaponComponent* weaponComponent = MeshComp->GetOwner()->GetComponentByClass<UWeaponComponent>())
	{
		if(weaponComponent->GetAttachment().IsValid())
			weaponComponent->GetAttachment()->SetCollision(ECollisionEnabled::QueryOnly);
	}
}

void UAttachmentCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (UWeaponComponent* weaponComponent = MeshComp->GetOwner()->GetComponentByClass<UWeaponComponent>())
	{
		if (weaponComponent->GetAttachment().IsValid())
			weaponComponent->GetAttachment()->SetCollision(ECollisionEnabled::NoCollision);
	}
}
