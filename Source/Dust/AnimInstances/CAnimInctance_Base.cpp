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

	CLog::Print(FString("Delta"), 2, 10, FColor::Red);
	CLog::Print(delta, 1, 10, FColor::Red);
	CLog::Print(FString("absoluteRotate"), 3, 10, FColor::Blue);
	CLog::Print(absoluteRotate, 4, 10, FColor::Blue);

	// �ڿ������� ȸ���� ���� ����
	InterpRotation = UKismetMathLibrary::RInterpTo(InterpRotation, delta, DeltaSeconds, 25);

	CLog::Print(FString("InterpRotation"), 5, 10, FColor::Green);
	CLog::Print(InterpRotation, 8, 10, FColor::Green);
	Direction = InterpRotation.Yaw;
	CLog::Print(FString("Direction"), 6, 10, FColor::Cyan);
	CLog::Print(Direction, 7, 10, FColor::Cyan);
}
