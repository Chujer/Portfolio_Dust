#include "GameObject/Weapon/Attachment.h"

#include "CLog.h"
#include "Action/CDoAction.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

AAttachment::AAttachment()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	WeaponMesh1 = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh1");
}

void AAttachment::DoAction_Server_Implementation()
{
	if (!DoAction.IsValid())
		return;
	DoAction->DoAction();
	DoAction_NMC();
}


void AAttachment::DoAction_NMC_Implementation()
{
	DoAction->PlayMontage();
}

void AAttachment::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());


	if (DoActionClass == nullptr|| OwnerCharacter == nullptr)
		return;
	
	DoAction = NewObject<UCDoAction>(this, DoActionClass);
	DoAction->BeginPlay(OwnerCharacter.Get());

	WeaponMesh1->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
	
}
