// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/StartEvadeToCameraFix.h"

#include "Character/CPlayerCharacter.h"

void UStartEvadeToCameraFix::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ACPlayerCharacter* OwnerCharacter = Cast<ACPlayerCharacter>(MeshComp->GetOwner());
	if(OwnerCharacter != nullptr)
		OwnerCharacter->DoEvadeToCameraFix(true);
}
