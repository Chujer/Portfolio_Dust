#include "Component/WeaponComponent.h"

#include "CLog.h"
#include "StateComponent.h"
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

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UWeaponComponent, CurAttachment);
}


void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponComponent::EndDoAction_Implementation()
{
	GetAttachment()->EndDoAction_Server();
}

void UWeaponComponent::DoAction_Combo_Implementation()
{
	if (!CurAttachment.IsValid())
		return;

	CurAttachment->DoActionCombo_Server();
}

void UWeaponComponent::DoAction_Implementation()
{
	if (!CurAttachment.IsValid())
		return;

	if (CurAttachment->IsComboReady()) //�޺����� ���� �Ⱓ�� Ŭ���� �޺����� ��⼳��
		CurAttachment->SetIsNext(true);

	if (StateComponent->GetStateType() != EStateType::Idle)
		return;
	
	CurAttachment->StartAction_Server();
}

void UWeaponComponent::SetWeaponAnimInstance_Implementation(UWeaponDataAsset* weaponData)
{
	if (!OwnerCharacter.IsValid())
		return;

	//�÷��̾� �ִ� �ν��Ͻ��� ���⿡ ���� ����
	OwnerCharacter->GetMesh()->SetAnimClass(weaponData->AnimInstance);
	OwnerCharacter->GetMesh()->GetAnimInstance()->NativeBeginPlay();
	
}

void UWeaponComponent::SetWeaponData_Implementation(UWeaponDataAsset* weaponData)
{
	CurWeaponData = weaponData;
	if (!weaponData)
	{
		CLog::Print("weaponData����");
		return;
	}
	if (CurAttachment != nullptr)
	{
		CurAttachment->Destroy();
	}

	FActorSpawnParameters param;
	param.Owner = Cast<AActor>(OwnerCharacter);

	CurAttachment = GetWorld()->SpawnActor<AAttachment>(weaponData->Attachment, param);
	
	SetWeaponAnimInstance(weaponData);
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

