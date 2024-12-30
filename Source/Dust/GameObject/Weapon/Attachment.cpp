#include "GameObject/Weapon/Attachment.h"

#include "CLog.h"
#include "Action/CDoAction.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

AAttachment::AAttachment()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
;
	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	WeaponMesh1 = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh1");
	Collision = CreateDefaultSubobject<UCapsuleComponent>("Collision");
	Collision->SetupAttachment(WeaponMesh1);
}

void AAttachment::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
	if (OwnerCharacter == nullptr)
		return;

	WeaponMesh1->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
	

}

void AAttachment::Temp()
{
}
