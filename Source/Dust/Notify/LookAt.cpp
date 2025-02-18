// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/LookAt.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CBaseCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

void ULookAt::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                     const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ACBaseCharacter* OwnerCharacter = Cast<ACBaseCharacter>(MeshComp->GetOwner());
	
	if (OwnerCharacter == nullptr)
		return;

	if (AAIController* aiController = Cast<AAIController>(OwnerCharacter->GetController()))
	{
		AActor* target = Cast<AActor>(aiController->GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		if (target == nullptr)
			return;

		OwnerCharacter->LookAtTarget(target);
	}
}
