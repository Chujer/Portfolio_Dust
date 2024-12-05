#include "GameObject/Weapon/Attachment.h"

#include "CLog.h"
#include "Action/CDoAction.h"
#include "Component/StateComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

AAttachment::AAttachment()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
;
	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	WeaponMesh1 = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh1");
}

void AAttachment::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	MaxActionIndex = DoActionDatas.Num();
	
	if (DoActionClass == nullptr || OwnerCharacter == nullptr)
		return;

	DoAction = NewObject<UCDoAction>(this, DoActionClass);
	DoAction->BeginPlay(OwnerCharacter.Get());

	WeaponMesh1->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);

}

void AAttachment::StartAction_Server()
{
	if (!DoAction.IsValid())
		return;

	DoAction->DoAction();

	//사용할 DoActionData의 Index 증가
	DoAction_NMC(DoActionDatas[ActionIndex]);
}

void AAttachment::EndDoAction_Server()
{
	DoAction->EndDoAtion();
	ActionIndex = 0;
	IsNext = false;
	ComboReady = false;
}

void AAttachment::DoActionCombo_Server()
{
	DoAction_NMC(DoActionDatas[ActionIndex++]);

	if (ActionIndex >= MaxActionIndex)
		ActionIndex = 0;
}

void AAttachment::DoAction_NMC_Implementation(const FDoActionData& DoActionData)
{
	DoAction->PlayMontage(DoActionData);
}
