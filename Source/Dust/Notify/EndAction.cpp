// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/EndAction.h"

#include "Character/CPlayerCharacter.h"
#include "Component/IdentityComponent.h"
#include "Component/MoveComponent.h"
#include "Component/StateComponent.h"
#include "Component/WeaponComponent.h"


void UEndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (UMoveComponent* moveComponent = MeshComp->GetOwner()->GetComponentByClass<UMoveComponent>())
	{
		moveComponent->SetStop(false);
		moveComponent->SetCamerafix(false);
	}

	if (!MeshComp->GetOwner()->HasAuthority())
		return;

	if(Cast<ACPlayerCharacter>(MeshComp->GetOwner()))
		Cast<ACPlayerCharacter>(MeshComp->GetOwner())->IsUseControllerRotYaw = true;

	if (UWeaponComponent* weaponComponent = MeshComp->GetOwner()->GetComponentByClass<UWeaponComponent>())
	{
		weaponComponent->EndDoAction_Server();
	}

	if(UIdentityComponent* identityComponent = MeshComp->GetOwner()->GetComponentByClass<UIdentityComponent>())
	{
		identityComponent->EndIdentitySkill();
	}

}
