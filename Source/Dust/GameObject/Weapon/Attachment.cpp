#include "GameObject/Weapon/Attachment.h"

#include "CLog.h"
#include "GameFramework/Character.h"

AAttachment::AAttachment()
{
	PrimaryActorTick.bCanEverTick = true;

	
	WeaponMesh1 = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh1");
}

void AAttachment::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (OwnerCharacter == nullptr)
		return;
	
	WeaponMesh1->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
}
