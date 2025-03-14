// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/SetControllerYaw.h"

#include "Character/CBaseCharacter.h"

void USetControllerYaw::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                               const FAnimNotifyEventReference& EventReference)
{
    if(ACBaseCharacter* Character = Cast<ACBaseCharacter>(MeshComp->GetOwner()))
		Character->bUseControllerRotationYaw = isUseControllerYaw;
}
