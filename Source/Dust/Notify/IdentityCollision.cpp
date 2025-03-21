// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/IdentityCollision.h"

#include "Component/IdentityComponent.h"
#include "GameObject/Weapon/IdentityObject.h"


void UIdentityCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp->GetOwner()->HasAuthority())
		return;

	if (UIdentityComponent* identityComponent = MeshComp->GetOwner()->GetComponentByClass<UIdentityComponent>())
	{
		if (identityComponent->GetIdentity() != nullptr)
			identityComponent->GetIdentity()->SetCollision(ECollisionEnabled::QueryOnly);
	}
}

void UIdentityCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp->GetOwner()->HasAuthority())
		return;

	if (UIdentityComponent* identityComponent = MeshComp->GetOwner()->GetComponentByClass<UIdentityComponent>())
	{

		if (identityComponent->GetIdentity() != nullptr)
			identityComponent->GetIdentity()->SetCollision(ECollisionEnabled::NoCollision);
	}
}
