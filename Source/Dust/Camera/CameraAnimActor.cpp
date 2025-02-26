// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraAnimActor.h"

#include "CameraAnimationSequence.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void ACameraAnimActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACameraAnimActor::StartCameraAnim_Implementation(UCameraAnimationSequence* AnimSequence)
{
}

void ACameraAnimActor::EndCameraAnim_Implementation()
{
}


