#include "GameObject/Weapon/Attachment.h"

#include "CLog.h"
#include "Action/CDoAction.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AAttachment::AAttachment()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
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
	
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh1->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AAttachment::BeginOverlap);
}

void AAttachment::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//公扁面倒 贸府
	if (OtherActor == OwnerCharacter || Cast<ACharacter>(OtherActor) == nullptr)
		return;
	if (OwnerCharacter->GetClass() == OtherActor->GetClass())
		return;

	Super::NotifyActorBeginOverlap(OtherActor);

	if (OnBeginCollision.IsBound())
	{
		TArray<AActor*> ignore;
		FHitResult HitResult;
		ignore.AddUnique(OwnerCharacter.Get());
		UKismetSystemLibrary::LineTraceSingle(this, Collision->GetComponentLocation(), OtherActor->GetActorLocation(), ETraceTypeQuery::TraceTypeQuery3, false,
			ignore, EDrawDebugTrace::Type::ForDuration, HitResult, true);
		OnBeginCollision.Broadcast(OtherActor, this, HitResult);
	}
}
