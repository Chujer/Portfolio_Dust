// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/CAnimInctance_Base.h"

#include "CLog.h"
#include "Utility/CHelpers.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

void UCAnimInctance_Base::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Character = Cast<ACharacter>(TryGetPawnOwner());

}

void UCAnimInctance_Base::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Character.IsValid())
		return;

	Speed = Character->GetVelocity().Size2D();

	//맵 자체 절대값 기준 캐릭터 이동 방향 - 현재 캐릭터가 보는 방향
	FRotator absoluteRotate = Character->GetVelocity().Rotation();
	
	FRotator characterRotate = Character->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(absoluteRotate, characterRotate);
	
	// 자연스러운 회전을 위한 보간
	InterpRotation = UKismetMathLibrary::RInterpTo(InterpRotation, delta, DeltaSeconds, 25);
	
	Direction = InterpRotation.Yaw;
}
