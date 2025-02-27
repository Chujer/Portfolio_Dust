// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraAnimActor.h"

#include "GameplayCamerasSubsystem.h"
#include "CameraAnimationSequence.h"
#include "GameplayCamerasClasses.h"
#include "GameFramework/Character.h"

// Called when the game starts or when spawned
void ACameraAnimActor::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void ACameraAnimActor::StartCameraAnim_Implementation(UCameraAnimationSequence* AnimSequence)
{
	//APlayerController* controller = Cast<APlayerController>(Cast<ACharacter>(GetOwner())->GetController());
	//FCameraAnimationParams param;
	 
	//UGameplayCamerasSubsystem* CamerasSubsystem = UGameplayCamerasSubsystem::GetGameplayCamerasSubsystem(GetWorld());
	//CamerasSubsystem->PlayCameraAnimation(controller, AnimSequence, param);
}

void ACameraAnimActor::EndCameraAnim_Implementation()
{
}


