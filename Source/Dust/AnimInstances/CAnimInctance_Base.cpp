// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/CAnimInctance_Base.h"

#include "Character/CBaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UCAnimInctance_Base::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACBaseCharacter>(TryGetPawnOwner());

}

void UCAnimInctance_Base::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwnerCharacter.IsValid())
		return;

	if(UFeetComponent* FeetComponent = OwnerCharacter->GetComponentByClass<UFeetComponent>())
	{
		FeetData = FeetComponent->GetData();
	}

	Speed = OwnerCharacter->GetVelocity().Size2D();

	//�� ��ü ���밪 ���� ĳ���� �̵� ���� - ���� ĳ���Ͱ� ���� ����
	FRotator absoluteRotate = OwnerCharacter->GetVelocity().Rotation();
	
	FRotator characterRotate = OwnerCharacter->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(absoluteRotate, characterRotate);
	
	// �ڿ������� ȸ���� ���� ����
	InterpRotation = UKismetMathLibrary::RInterpTo(InterpRotation, delta, DeltaSeconds, 25);
	
	Direction = InterpRotation.Yaw;
}
