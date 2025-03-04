// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/StartCameraShake.h"

#include "GameFramework/Character.h"

void UStartCameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                               const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ACharacter* ownerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (ownerCharacter == nullptr)
		return;
	APlayerController* controller = Cast<APlayerController>(ownerCharacter->GetController());
	if (controller == nullptr || CameraShakeClass == nullptr)
		return;

	controller->PlayerCameraManager->StartCameraShake(CameraShakeClass);
}
