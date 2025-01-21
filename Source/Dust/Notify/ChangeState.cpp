// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/ChangeState.h"

#include "Component/StateComponent.h"

void UChangeState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	

	if (UStateComponent* StateComponent = MeshComp->GetOwner()->GetComponentByClass<UStateComponent>())
	{
		StateComponent->ChangeType(StateType);
	}
}
