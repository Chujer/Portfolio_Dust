#include "Component/WeaponComponent.h"

#include "CLog.h"
#include "StateComponent.h"
#include "DataAsset/WeaponData.h"
#include "DataAsset/WeaponDataAsset.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


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
	if (GetDoAction() == nullptr)
		return;

	if (StateComponent->GetStateType() != EStateType::Idle)
		return;
	CLog::Print(GetDoAction()->GetName(), 2, 10, FColor::Yellow);

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


void UWeaponComponent::SetWeaponData_NMC_Implementation(class UWeaponDataAsset* weaponDataAsset, AAttachment* Attachment)
{
	WeaponDataAsset = weaponDataAsset;

	//������ ���������͸� ������� �ʱ����� ���ο� ���� �����͸� ����
	//UObject(UWeaponData)�� ��� ���ø����̼��� ���� �ʾ� �� Ŭ���̾�Ʈ�鿡 ����
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

void UWeaponComponent::SetWeaponData_Server_Implementation(UWeaponDataAsset* weaponDataAsset)
{
	if (weaponDataAsset == nullptr)
	{
		CLog::Print("weaponDataAsset����");
		return;
	}

	FActorSpawnParameters param;
	param.Owner = Cast<AActor>(OwnerCharacter);
	//Attachment�� ��� ���ø����̼��� �����̹Ƿ� ��(Server)���� ������ �Ű������� ����
	SetWeaponData_NMC(weaponDataAsset, GetWorld()->SpawnActor<AAttachment>(weaponDataAsset->AttachmentClass, param));
}

//void UWeaponComponent::SetWeaponData(UWeaponDataAsset* weaponData)
//{
//	if (!weaponData)
//	{
//		CLog::Print("weaponData����");
//		return;
//	}
//	if (CurAttachment != nullptr)
//		CurAttachment->Destroy();
//
//	FActorSpawnParameters param;
//	param.Owner = Cast<AActor>(OwnerCharacter);
//	
//	CurAttachment = GetWorld()->SpawnActor<AAttachment>(weaponData->Attachment, param);
//
//	//�÷��̾� �ִ� �ν��Ͻ��� ���⿡ ���� ����
//	OwnerCharacter->GetMesh()->SetAnimClass(weaponData->AnimInstance);
//	OwnerCharacter->GetMesh()->GetAnimInstance()->NativeBeginPlay();
//}

