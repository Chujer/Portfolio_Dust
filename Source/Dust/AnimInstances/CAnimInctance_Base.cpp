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

	//�� ��ü ���밪 ���� ĳ���� �̵� ���� - ���� ĳ���Ͱ� ���� ����
	FRotator absoluteRotate = Character->GetVelocity().Rotation();
	
	FRotator characterRotate = Character->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(absoluteRotate, characterRotate);
	
	// �ڿ������� ȸ���� ���� ����
	InterpRotation = UKismetMathLibrary::RInterpTo(InterpRotation, delta, DeltaSeconds, 25);
	
	Direction = InterpRotation.Yaw;
}
