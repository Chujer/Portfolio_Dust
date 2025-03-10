// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/SetMoveStop.h"

#include "Component/MoveComponent.h"

void USetMoveStop::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (UMoveComponent* MoveComponent = MeshComp->GetOwner()->GetComponentByClass<UMoveComponent>())
	{
		MoveComponent->SetStop(true);
	}	
}
