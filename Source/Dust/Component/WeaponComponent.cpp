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

	if (ACLobbyController* lobbyController = Cast<ACLobbyController>(OwnerCharacter->GetController()))	//�κ񿡼� ���⸦ ������ ����
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
	
	//Attachment�� ��� ���ø����̼��� �����̹Ƿ� ��(Server)���� ������ �Ű������� ����
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

	//���� curWeaponIndex�� Replication�� ����Ϸ� ������ 
	//���� ������ Server���� �̷�� �������� ���� �Լ��� ����Ǵ� ������ �߻��� ���� �������� ����
	curWeaponIndex = WeaponIndex;

	if (Cast<ACPlayerCharacter>(OwnerCharacter))
	{
		Cast<ACPlayerCharacter>(OwnerCharacter)->SetRotateOption();
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

		//����� WeaponIndex���� Save
		if (OwnerCharacter.IsValid() && OwnerCharacter->IsLocallyControlled())
			OwnerCharacter->GetComponentByClass<UPlayerSaveComponent>()->SaveData();
	}
	FString temp = FString::FromInt(WeaponIndex);
	FWeaponDataStruct* weaponDataRow = DataTable->FindRow<FWeaponDataStruct>(FName(*temp), FString(""));

	WeaponDataAsset = weaponDataRow->WeaponDataAsset;

	//������ ���������͸� ������� �ʱ����� ���ο� ���� �����͸� ����
	//UObject(UWeaponData)�� ��� ���ø����̼��� ���� �ʾ� �� Ŭ���̾�Ʈ�鿡 ���� �� �� �Ҵ�
	WeaponData = NewObject<UWeaponData>(this, UWeaponData::StaticClass());
	WeaponData->Attachment = tempAttachment;
	WeaponData->AnimInstance = WeaponDataAsset->AnimInstance;

	if (!!WeaponDataAsset->DoActionClass)
	{
		WeaponData->DoAction = NewObject<UCDoAction>(this, WeaponDataAsset->DoActionClass);
		WeaponData->DoAction->BeginPlay(OwnerCharacter.Get());
		//�ݸ��� �浹 ��������Ʈ ����
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

	//�÷��̾� �ִ� �ν��Ͻ��� ���⿡ ���� ����
	OwnerCharacter->GetMesh()->SetAnimClass(WeaponDataAsset->AnimInstance);
	OwnerCharacter->GetMesh()->GetAnimInstance()->NativeBeginPlay();
}

void UWeaponComponent::OnRepAttach()
{
	SetWeaponData(curWeaponIndex, tempAttachment);
}

void UWeaponComponent::LoadSetWeaponData()
{
	//SaveData���� ���� �޾ƿ� ���� ����
	if (SaveComponent.IsValid())
		SaveComponent->LoadSetWeaponData();
}