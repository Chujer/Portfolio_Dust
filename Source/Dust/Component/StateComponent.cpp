// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StateComponent.h"

#include "AIController.h"
#include "CLog.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Character/CBaseCharacter.h"
#include "Character/CEnemyCharacter.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Widget/CGroggyWidget.h"
#include "Widget/CHPWidget.h"

UStateComponent::UStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UStateComponent, Type);
	DOREPLIFETIME(UStateComponent, HP);
	DOREPLIFETIME(UStateComponent, MaxHP);
	DOREPLIFETIME(UStateComponent, GroggyTime);
}


void UStateComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACBaseCharacter>(GetOwner());

	if (OwnerCharacter->GetController() == nullptr)
		return;

	HP = MaxHP;
	OnStateTypeChanged.AddDynamic(this, &UStateComponent::OnHittingParry);
	OnStateTypeChanged.AddDynamic(this, &UStateComponent::OnGroggy);
	OnStateTypeChanged.AddDynamic(this, &UStateComponent::OnDown);
	OnStateTypeChanged.AddDynamic(this, &UStateComponent::OnExecute);
}

void UStateComponent::PlayAnimMontage_NMC_Implementation(UAnimMontage* montage)
{
	OwnerCharacter->PlayAnimMontage(montage);
}

void UStateComponent::MakeHPUI()
{
	if(HPWidgetClass == nullptr)
		return;

	HPWidget = Cast<UCHPWidget>(CreateWidget(GetWorld(), HPWidgetClass));
	HPWidget->AddToViewport();
	HPWidget->MaxHP = MaxHP;
	HPWidget->HP = HP;
}

void UStateComponent::ChangeType_Implementation(EStateType InType)
{
	EStateType prevType = Type;
	Type = InType;

	if(AAIController* aiController = Cast<AAIController>(OwnerCharacter->GetController()))
		aiController->GetBlackboardComponent()->SetValueAsEnum("State",  (uint8)Type);

	//상태 변경시 델리게이트에 설정한 함수를 실행
	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, Type);
}

void UStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UStateComponent::SetRollMode()
{
	ChangeType(EStateType::Roll);
}

void UStateComponent::SetGroggyMode()
{
	ChangeType(EStateType::Groggy);
}

void UStateComponent::SetHittingParryMode()
{
	ChangeType(EStateType::HittingParry);
}

void UStateComponent::SetExecuteMode()
{
	ChangeType(EStateType::Execute);
}

void UStateComponent::SetDownMode()
{
	ChangeType(EStateType::Down);
}

void UStateComponent::SubHP(float Damage)
{
	HP -= Damage;
	if (HP <= 0)
	{
		SetDeadMode();
	}
}

void UStateComponent::OnHittingParry(EStateType InPrevType, EStateType InNewType)
{
	if (InNewType == EStateType::HittingParry)
	{
		OwnerCharacter->PlayMontage_Server(OwnerCharacter->ParryHitAnimation);
	}
}

void UStateComponent::OnGroggy(EStateType InPrevType, EStateType InNewType)
{
	if (InNewType != EStateType::Groggy)
		return;

	if (!OwnerCharacter->HasAuthority())
		return;

	//그로기 상태로 진입 3초후 기존 상태로 복구
	GetWorld()->GetTimerManager().SetTimer(GroggyTimer, this, &UStateComponent::EndGroggy, 3.0f, false, 3.0f);
}

void UStateComponent::EndGroggy()
{
	OwnerCharacter->PlayMontage_Server();
	SetIdleMode();
	GetWorld()->GetTimerManager().ClearTimer(GroggyTimer);
}

void UStateComponent::OnDown(EStateType InPrevType, EStateType InNewType)
{
	if (InNewType != EStateType::Down)
		return;
	if (!OwnerCharacter->HasAuthority())
		return;


	//다운 상태 진입 2초후 기존 상태로 복구
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UStateComponent::EndDown, 2.0f, false, 3.0f);
}

void UStateComponent::EndDown()
{
	OwnerCharacter->PlayMontage_Server(OwnerCharacter->EndDownAnimation);
	GetWorld()->GetTimerManager().ClearTimer(Timer);
}

void UStateComponent::OnExecute(EStateType InPrevType, EStateType InNewType)
{
	if (InPrevType == EStateType::Groggy && InNewType == EStateType::Execute)
	{
		GetWorld()->GetTimerManager().ClearTimer(GroggyTimer);
	}
}

void UStateComponent::SetGroggyWidget(UUserWidget* Widget)
{
	if (UWidgetComponent* temp = OwnerCharacter->GetComponentByClass<UWidgetComponent>())
	{
		if (temp->GetUserWidgetObject() != nullptr)
			CLog::Print(temp->GetUserWidgetObject()->GetName());
	}

	GroggyWidget = Cast<UCGroggyWidget>(Widget);
	if(GroggyWidget != nullptr)
		GroggyWidget->MaxGroggyTime = MaxGroggyTime;
	
}


void UStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (HPWidget == nullptr)
		return;
	HPWidget->HP = HP;



	if (Cast<ACEnemyCharacter>(OwnerCharacter) == nullptr)
		return;

	// 타이머 함수 호출할 때 가지 남은 시간
	if (OwnerCharacter->HasAuthority())
	{
		GroggyTime = GetWorld()->GetTimerManager().GetTimerRemaining(GroggyTimer);
	}
	if(GroggyWidget != nullptr)
		GroggyWidget->GroggyTime = GroggyTime;
}

