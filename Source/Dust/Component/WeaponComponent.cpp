#include "Component/WeaponComponent.h"

#include "CLog.h"
#include "IdentityComponent.h"
#include "PlayerSaveComponent.h"
#include "StateComponent.h"
#include "Character/CPlayerCharacter.h"
#include "DataAsset/WeaponData.h"
#include "DataAsset/WeaponDataAsset.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Controller/CLobbyController.h>

#include "GameMode/CLobbyGameMode.h"


UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACBaseCharacter>(GetOwner());
	if (!OwnerCharacter.IsValid())
		return;

	StateComponent = OwnerCharacter->GetComponentByClass<UStateComponent>();
	SaveComponent  = OwnerCharacter->GetComponentByClass<UPlayerSaveComponent>();
}

void UWeaponComponent::BeginDestroy()
{
	Super::BeginDestroy();
}

class AAttachment* UWeaponComponent::GetAttachment() const
{
	if (WeaponData == nullptr)
		return nullptr;

	return WeaponData->Attachment;
}

class UCDoAction* UWeaponComponent::GetDoAction() const
{
	if (WeaponData == nullptr)
		return nullptr;

	return WeaponData->DoAction;
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWeaponComponent, curWeaponIndex);
	DOREPLIFETIME(UWeaponComponent, tempAttachment);
}


void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponComponent::DoActionRight_Server_Implementation()
{
	if (GetDoAction() == nullptr)
		return;
	GetDoAction()->DoActionRight_Server();
	DoActionRight_NMC();
}

void UWeaponComponent::DoActionRight_NMC_Implementation()
{
	GetDoAction()->DoActionRight_NMC();
}

void UWeaponComponent::DoIndexAction_Server_Implementation(int Index)
{
	if (StateComponent->GetStateType() == EStateType::Action)
		return;

	if (GetDoAction() == nullptr)
		return;

	if (WeaponData == nullptr)
		return;

	GetDoAction()->DoAction_Server();
	DoIndexAction_NMC(Index);
}

void UWeaponComponent::DoIndexAction_NMC_Implementation(int Index)
{
	if (GetDoAction() == nullptr)
		return;

	GetDoAction()->DoIndexAction_NMC(Index);
}

void UWeaponComponent::DoAction_Server_Implementation()
{
	if (GetDoAction() == nullptr)
		return;
	if (curWeaponIndex == 0)
		return;
	
	GetDoAction()->DoActionTrigger();

	if (!StateComponent->IsIdleMode())
		return;

	GetDoAction()->DoAction_Server();
	DoAction_NMC();
}

void UWeaponComponent::DoAction_NMC_Implementation()
{
	GetDoAction()->DoAction_NMC();
}

void UWeaponComponent::EndDoAction_Server_Implementation()
{
	if (GetDoAction() == nullptr)
		return;

	GetDoAction()->EndDoAtion_Server();
	EndDoAction_NMC();
	GetAttachment()->ClearHittedCharacter();
}

void UWeaponComponent::SetWeaponData_Server_Implementation(int WeaponIndex)
{
	if (DataTable == nullptr || WeaponIndex > DataTable->GetRowNames().Num() || WeaponIndex == 0)
	{
		return;
	}
	
	curWeaponIndex = WeaponIndex;
	FString temp = FString::FromInt(WeaponIndex);

	FWeaponDataStruct* weaponDataRow = DataTable->FindRow<FWeaponDataStruct>(FName(*temp), FString(""));

	if (ACLobbyController* lobbyController = Cast<ACLobbyController>(OwnerCharacter->GetController()))	//로비에서 무기를 장착시 변경
	{
		if (weaponDataRow->WeaponDataAsset != nullptr && weaponDataRow->WeaponDataAsset->Icon != nullptr)
		{
			lobbyController->PlayerInfo.WeaponIcon = weaponDataRow->WeaponDataAsset->Icon;
			if (auto lobbyGameMode = Cast<ACLobbyGameMode>(GetWorld()->GetAuthGameMode()))
				lobbyGameMode->UpdatePlayerInfo();
		}
	}

	FActorSpawnParameters param;
	param.Owner = Cast<AActor>(OwnerCharacter);
	
	//Attachment의 경우 리플리케이션한 엑터이므로 밖(Server)에서 생성후 매개변수로 전달
	tempAttachment = GetWorld()->SpawnActor<AAttachment>(weaponDataRow->WeaponDataAsset->AttachmentClass, param);

	if(UIdentityComponent* IdentityComponent = OwnerCharacter->GetComponentByClass<UIdentityComponent>())
	{
		IdentityComponent->SetIdentity(weaponDataRow->WeaponDataAsset->IdentityClass);
	}
	
	OnRepAttach();
}

void UWeaponComponent::SetWeaponData(int WeaponIndex, AAttachment* Attachment)
{
	if (!OwnerCharacter.IsValid())
		return;

	//기존 curWeaponIndex를 Replication해 사용하려 했으나 
	//값의 변경이 Server에서 이루어 지기전에 저장 함수가 실행되는 문제가 발생해 값을 수동으로 변경
	curWeaponIndex = WeaponIndex;

	if (Cast<ACPlayerCharacter>(OwnerCharacter))
	{
		Cast<ACPlayerCharacter>(OwnerCharacter)->SetRotateOption();
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

		//변경된 WeaponIndex값을 Save
		if (OwnerCharacter.IsValid() && OwnerCharacter->IsLocallyControlled())
			OwnerCharacter->GetComponentByClass<UPlayerSaveComponent>()->SaveData();
	}
	FString temp = FString::FromInt(WeaponIndex);
	FWeaponDataStruct* weaponDataRow = DataTable->FindRow<FWeaponDataStruct>(FName(*temp), FString(""));

	WeaponDataAsset = weaponDataRow->WeaponDataAsset;

	//동일한 웨폰데이터를 사용하지 않기위해 새로운 웨폰 데이터를 생성
	//UObject(UWeaponData)의 경우 리플리케이션이 되지 않아 각 클라이언트들에 생성 및 값 할당
	WeaponData = NewObject<UWeaponData>(this, UWeaponData::StaticClass());
	WeaponData->Attachment = tempAttachment;
	WeaponData->AnimInstance = WeaponDataAsset->AnimInstance;

	if (!!WeaponDataAsset->DoActionClass)
	{
		WeaponData->DoAction = NewObject<UCDoAction>(this, WeaponDataAsset->DoActionClass);
		WeaponData->DoAction->BeginPlay(OwnerCharacter.Get());
		//콜리전 충돌 델리게이트 설정
		WeaponData->Attachment->OnBeginCollision.AddDynamic(WeaponData->DoAction, &UCDoAction::ApplyDamage);
	}
	if (!OwnerCharacter.IsValid())
		return;
	SetWeaponAnimInstance_NMC();
}

void UWeaponComponent::EndDoAction_NMC_Implementation()
{
	if (GetDoAction() == nullptr)
		return;
	GetDoAction()->EndDoAtion_NMC();
}

void UWeaponComponent::SetWeaponAnimInstance_NMC()
{
	if (!OwnerCharacter.IsValid())
		return;

	//플레이어 애님 인스턴스를 무기에 맞춰 변경
	OwnerCharacter->GetMesh()->SetAnimClass(WeaponDataAsset->AnimInstance);
	OwnerCharacter->GetMesh()->GetAnimInstance()->NativeBeginPlay();
}

void UWeaponComponent::OnRepAttach()
{
	SetWeaponData(curWeaponIndex, tempAttachment);
}

void UWeaponComponent::LoadSetWeaponData()
{
	//SaveData에서 값을 받아와 무기 설정
	if (SaveComponent.IsValid())
		SaveComponent->LoadSetWeaponData();
}