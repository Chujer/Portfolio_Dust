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

void UWeaponComponent::SetWeaponData(UWeaponDataAsset* weaponData)
{
	if (!weaponData)
	{
		CLog::Print("weaponData없음");
		return;
	}
	if (CurAttachment != nullptr)
		CurAttachment->Destroy();

	FActorSpawnParameters param;
	param.Owner = Cast<AActor>(OwnerCharacter);
	
	CurAttachment = GetWorld()->SpawnActor<AAttachment>(weaponData->Attachment, param);

	//플레이어 애님 인스턴스를 무기에 맞춰 변경
	OwnerCharacter->GetMesh()->SetAnimClass(weaponData->AnimInstance);
	OwnerCharacter->GetMesh()->GetAnimInstance()->NativeBeginPlay();
}

