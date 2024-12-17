#include "Component/WeaponComponent.h"

#include "CLog.h"
#include "PlayerSaveComponent.h"
#include "StateComponent.h"
#include "DataAsset/WeaponData.h"
#include "DataAsset/WeaponDataAsset.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "DataAsset/WeaponData.h"
#include "DataAsset/WeaponDataAsset.h"


UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter.IsValid())
		return;

	StateComponent = OwnerCharacter->GetComponentByClass<UStateComponent>();
	SaveComponent  = OwnerCharacter->GetComponentByClass<UPlayerSaveComponent>();
}

TWeakObjectPtr<AAttachment> UWeaponComponent::GetAttachment() const
{
	if (WeaponData == nullptr)
		return nullptr;

	return WeaponData->Attachment;
}

TWeakObjectPtr<UCDoAction> UWeaponComponent::GetDoAction() const
{
	if (WeaponData == nullptr)
		return nullptr;

	return WeaponData->DoAction;
}


void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UWeaponComponent::DoAction_Server_Implementation()
{
	if (!GetDoAction().IsValid())
		return;
	if (curWeaponIndex == 0)
		return;

	if (StateComponent->GetStateType() != EStateType::Idle)
		return;

	GetDoAction()->DoAction_Server();
	DoAction_NMC();
}

void UWeaponComponent::DoAction_NMC_Implementation()
{
	if (GetDoAction() == nullptr)
		return;

	GetDoAction()->DoAction_NMC();
}

void UWeaponComponent::EndDoAction_Server_Implementation()
{
	if (GetDoAction() == nullptr)
		return;

	GetDoAction()->EndDoAtion_Server() ;
}


void UWeaponComponent::SetWeaponData_NMC_Implementation(int WeaponIndex, AAttachment* Attachment)
{
	//���� curWeaponIndex�� Replication�� ����Ϸ� ������ 
	//���� ������ Server���� �̷�� �������� ���� �Լ��� ����Ǵ� ������ �߻��� NMC�� ���� �������� ����
	curWeaponIndex = WeaponIndex;
	FString temp = FString::FromInt(WeaponIndex);
	FWeaponDataStruct* weaponDataRow = DataTable->FindRow<FWeaponDataStruct>(FName(*temp), FString(""));
	
	WeaponDataAsset = weaponDataRow->WeaponDataAsset;

	//����� WeaponIndex���� Save
	if (OwnerCharacter.IsValid() && OwnerCharacter->IsLocallyControlled())
		OwnerCharacter->GetComponentByClass<UPlayerSaveComponent>()->SaveData();

	//������ ���������͸� ������� �ʱ����� ���ο� ���� �����͸� ����
	//UObject(UWeaponData)�� ��� ���ø����̼��� ���� �ʾ� �� Ŭ���̾�Ʈ�鿡 ���� �� �� �Ҵ�
	WeaponData = NewObject<UWeaponData>(this, UWeaponData::StaticClass());
	WeaponData->Attachment = Attachment;
	WeaponData->AnimInstance = WeaponDataAsset->AnimInstance;
	if (!!WeaponDataAsset->DoActionClass)
	{
		WeaponData->DoAction = NewObject<UCDoAction>(this, WeaponDataAsset->DoActionClass);
		WeaponData->DoAction->BeginPlay(OwnerCharacter.Get());
	}

	if (!OwnerCharacter.IsValid())
		return;
	SetWeaponAnimInstance_NMC();
}

void UWeaponComponent::SetWeaponAnimInstance_NMC()
{
	if (!OwnerCharacter.IsValid())
		return;

	//�÷��̾� �ִ� �ν��Ͻ��� ���⿡ ���� ����
	OwnerCharacter->GetMesh()->SetAnimClass(WeaponDataAsset->AnimInstance);
	OwnerCharacter->GetMesh()->GetAnimInstance()->NativeBeginPlay();
	
}

void UWeaponComponent::LoadSetWeaponData()
{
	//SaveData���� ���� �޾ƿ� ���� ����
	if (SaveComponent.IsValid())
		SaveComponent->LoadSetWeaponData();
}

void UWeaponComponent::SetWeaponData_Server_Implementation(int WeaponIndex)
{
	if (DataTable == nullptr || WeaponIndex > DataTable->GetRowNames().Num() || WeaponIndex == 0)
	{
		CLog::Print("WeaponIndex > DataTableSize");
		return;
	}
	curWeaponIndex = WeaponIndex;
	FString temp = FString::FromInt(WeaponIndex);

	FWeaponDataStruct* weaponDataRow = DataTable->FindRow<FWeaponDataStruct>(FName(*temp), FString(""));

	FActorSpawnParameters param;
	param.Owner = Cast<AActor>(OwnerCharacter);
	//Attachment�� ��� ���ø����̼��� �����̹Ƿ� ��(Server)���� ������ �Ű������� ����
	SetWeaponData_NMC(WeaponIndex, GetWorld()->SpawnActor<AAttachment>(weaponDataRow->WeaponDataAsset->AttachmentClass, param));
}
