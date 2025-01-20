#include "GameObject/Weapon/IdentityObject.h"

#include "Components/ShapeComponent.h"
#include "GameFramework/Character.h"

AIdentityObject::AIdentityObject()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	IdentityMesh = CreateDefaultSubobject<UStaticMeshComponent>("IdentityMesh");
}

void AIdentityObject::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (OwnerCharacter == nullptr)
		return;


	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
	IdentityMesh->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
}

void AIdentityObject::PlayMontage_Server_Implementation(UAnimMontage* montage)
{
	PlayMontage_NMC(montage);
}

void AIdentityObject::PlayMontage_NMC_Implementation(UAnimMontage* montage)
{
	if (OwnerCharacter == nullptr)
		return;
	OwnerCharacter->PlayAnimMontage(montage);
}

void AIdentityObject::BeginIdentity()
{
}

void AIdentityObject::EndIdentity()
{
}

void AIdentityObject::BeginIdentitySkill()
{
}

void AIdentityObject::EndIdentitySkill()
{
}

