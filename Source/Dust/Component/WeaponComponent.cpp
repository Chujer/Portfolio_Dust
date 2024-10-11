#include "Component/WeaponComponent.h"

#include "CLog.h"
#include "DataAsset/WeaponDataAsset.h"
#include "GameFramework/Character.h"


UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
}


void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UWeaponComponent::DoAction_Implementation()
{
	if (CurDoAction.IsValid())
		CurDoAction->DoAction();
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
		CurDoAction->ConditionalBeginDestroy();
	}

	FActorSpawnParameters param;
	param.Owner = Cast<AActor>(OwnerCharacter);

	CurAttachment = GetWorld()->SpawnActor<AAttachment>(weaponData->Attachment, param);
	CurDoAction = NewObject<UCDoAction>(this, weaponData->DoAction);
	CurDoAction->BeginPlay(OwnerCharacter.Get());
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

