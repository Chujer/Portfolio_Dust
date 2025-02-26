// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CameraAnimState.h"

#include "Camera/CameraAnimActor.h"
#include "GameFramework/Character.h"
#include "D:/UnrealEngine-5.3/UnrealEngine-5.3/Engine/Plugins/Cameras/GameplayCameras/Source/GameplayCameras/Public/GameplayCamerasSubsystem.h"

FString UCameraAnimState::GetNotifyName_Implementation() const
{
	return "CameraAnim";
}

void UCameraAnimState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());

	if (OwnerCharacter == nullptr || CameraAnimActorClass == nullptr || AnimSequence == nullptr)
		return;

	FRotator rotator = OwnerCharacter->GetActorRotation();
	rotator.Pitch = 0;
	FActorSpawnParameters param;
	param.Owner = Cast<AActor>(OwnerCharacter);

	CameraAnimActor = OwnerCharacter->GetWorld()->SpawnActor<ACameraAnimActor>(CameraAnimActorClass, OwnerCharacter->GetActorLocation(), rotator, param);
	CameraAnimActor->StartCameraAnim(AnimSequence);
}

void UCameraAnimState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (OwnerCharacter == nullptr)
		return;
	if (CameraAnimActor == nullptr)
		return;

	CameraAnimActor->EndCameraAnim();
}
