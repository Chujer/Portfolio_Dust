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

	DOREPLIFETIME(UWeaponComponent, curWeaponIndex);
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

	//동일한 웨폰데이터를 사용하지 않기위해 새로운 웨폰 데이터를 생성
	//UObject(UWeaponData)의 경우 리플리케이션이 되지 않아 각 클라이언트들에 생성
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

	//플레이어 애님 인스턴스를 무기에 맞춰 변경
	OwnerCharacter->GetMesh()->SetAnimClass(WeaponDataAsset->AnimInstance);
	OwnerCharacter->GetMesh()->GetAnimInstance()->NativeBeginPlay();
	
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
	//Attachment의 경우 리플리케이션한 엑터이므로 밖(Server)에서 생성후 매개변수로 전달
	SetWeaponData_NMC(weaponDataRow->WeaponDataAsset, GetWorld()->SpawnActor<AAttachment>(weaponDataRow->WeaponDataAsset->AttachmentClass, param));
}
