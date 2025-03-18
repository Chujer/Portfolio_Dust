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

	//실행 시킬 애니메이션의 방향으로 회전
	if (value.Y == 0 || value.X == 0)
	{
	}
	else if (value.Y > 0 && value.X != 0)
	{
		rotate.Yaw = OwnerCharacter->GetActorRotation().Yaw + UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::Atan2(value.X, value.Y));
		OwnerCharacter->SetActorRotation(rotate);

	}
	else if (value.Y < 0 && value.X != 0)
	{
		rotate.Yaw = OwnerCharacter->GetActorRotation().Yaw - (180 - UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::Atan2(value.X, value.Y)));
		OwnerCharacter->SetActorRotation(rotate);
	}
	
	//애니메이션의 종류가 4가지 이기 때문에 대각선은 앞또는 뒤애니메이션실행
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

