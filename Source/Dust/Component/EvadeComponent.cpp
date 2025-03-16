// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EvadeComponent.h"

#include "CLog.h"
#include "InputActionValue.h"
#include "StateComponent.h"
#include "Character/CPlayerCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

UEvadeComponent::UEvadeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UEvadeComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACBaseCharacter>(GetOwner());
	StateComponent = OwnerCharacter->GetComponentByClass<UStateComponent>();
	
}

void UEvadeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void UEvadeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UEvadeComponent::UpdateMoveAxis(const FInputActionValue& Value)
{
	MovementVector = Value.Get<FVector2D>();
}

void UEvadeComponent::PlayMontage(const FEvadeData& MontageData)
{
	if (MontageData.Montage == nullptr)
		return;
	OwnerCharacter->PlayMontage_Server(MontageData.Montage, MontageData.PlayRate);
}

void UEvadeComponent::Evade_Client_Implementation()
{
	if(OwnerCharacter.IsValid())
		OwnerCharacter->bUseControllerRotationYaw = false;
	Evade_Server(MovementVector);
}

void UEvadeComponent::Evade_Server_Implementation(const FVector2D& value)
{
	if (StateComponent == nullptr)
		return;

	if (!StateComponent->IsIdleMode())
		return;

	StateComponent->SetActionMode();
	
	FRotator rotate = OwnerCharacter->GetActorRotation();
	OwnerCharacter->bUseControllerRotationYaw = false;
	Cast<ACPlayerCharacter>(OwnerCharacter)->IsUseControllerRotYaw = false;

	//���� ��ų �ִϸ��̼��� �������� ȸ��
	if (value.Y == 0 || value.X == 0)
	{
	}
	else if (value.Y > 0 && value.X != 0)
	{
		rotate.Yaw = OwnerCharacter->GetActorRotation().Yaw + UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::Atan2(value.X, value.Y));
		CLog::Print(rotate.Yaw);
		OwnerCharacter->SetActorRotation(rotate);

	}
	else if (value.Y < 0 && value.X != 0)
	{
		rotate.Yaw = OwnerCharacter->GetActorRotation().Yaw - (180 - UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::Atan2(value.X, value.Y)));
		CLog::Print(rotate.Yaw);
		OwnerCharacter->SetActorRotation(rotate);
	}
	
	//�ִϸ��̼��� ������ 4���� �̱� ������ �밢���� �նǴ� �ھִϸ��̼ǽ���
	if (value.Y > 0)
		PlayMontage(EvadeDataF);
	else if (value.Y < 0)
		PlayMontage(EvadeDataB);
	else if (value.X > 0)
		PlayMontage(EvadeDataR);
	else if (value.X < 0)
		PlayMontage(EvadeDataL);
	else
		PlayMontage(EvadeDataF);
}


//void UEvadeComponent::Evade_Server_Implementation()
//{
//	if (StateComponent == nullptr)
//		return;
//
//	if (!StateComponent->IsIdleMode())
//		return;
//
//	StateComponent->SetActionMode();
//
//
//	Evade_NMC(MovementVector);
//}
//
//void UEvadeComponent::Evade_NMC_Implementation(const FVector2D& value)
//{
//	if (!OwnerCharacter.IsValid())
//		return;
//
//	FRotator rotate = OwnerCharacter->GetActorRotation();
//	OwnerCharacter->bUseControllerRotationYaw = false;
//	Cast<ACPlayerCharacter>(OwnerCharacter)->IsUseControllerRotYaw = false;
//
//
//	CLog::Print("TT : ", 6);
//	//if()
//	//���� ��ų �ִϸ��̼��� �������� ȸ��
//	if (OwnerCharacter->IsLocallyControlled())
//	{
//		if (MovementVector.Y == 0 || MovementVector.X == 0)
//		{
//		}
//		else if (MovementVector.Y > 0 && MovementVector.X != 0)
//		{
//			rotate.Yaw = OwnerCharacter->GetActorRotation().Yaw + UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::Atan2(MovementVector.X, MovementVector.Y));
//			CLog::Print(rotate.Yaw);
//			OwnerCharacter->SetActorRotation(rotate);
//		}
//		else if (MovementVector.Y < 0 && MovementVector.X != 0)
//		{
//			rotate.Yaw = OwnerCharacter->GetActorRotation().Yaw - (180 - UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::Atan2(MovementVector.X, MovementVector.Y)));
//			CLog::Print(rotate.Yaw);
//			OwnerCharacter->SetActorRotation(rotate);
//		}
//	}
//
//	//�ִϸ��̼��� ������ 4���� �̱� ������ �밢���� �նǴ� �ھִϸ��̼ǽ���
//	if (MovementVector.Y > 0)
//		PlayMontage(EvadeDataF);
//	else if (MovementVector.Y < 0)
//		PlayMontage(EvadeDataB);
//	else if (MovementVector.X > 0)
//		PlayMontage(EvadeDataR);
//	else if (MovementVector.X < 0)
//		PlayMontage(EvadeDataL);
//	else
//		PlayMontage(EvadeDataF);
//	
//}


