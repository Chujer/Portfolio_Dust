#include "GameObject/Weapon/IdentityObject.h"

#include "CLog.h"
#include "Character/CBaseCharacter.h"
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

	OwnerCharacter = Cast<ACBaseCharacter>(GetOwner());

	if (OwnerCharacter == nullptr)
		return;


	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
	IdentityMesh->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
}

void AIdentityObject::SetCollision_Implementation(ECollisionEnabled::Type value)
{
	Collision->SetCollisionEnabled(value);
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

