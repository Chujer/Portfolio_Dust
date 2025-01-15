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
	IdentityMesh->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
}

void AIdentityObject::BeginIdentity()
{
}

void AIdentityObject::EndIdentity()
{
}

