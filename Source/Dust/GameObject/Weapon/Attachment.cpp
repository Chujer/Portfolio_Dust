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
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	Collision = CreateDefaultSubobject<UCapsuleComponent>("Collision");
	Collision->SetupAttachment(WeaponMesh);
}

void AAttachment::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
		return;
	
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
	WeaponMesh->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AAttachment::BeginOverlap);
}

void AAttachment::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	ACharacter* otherActor = Cast<ACharacter>(OtherActor);

	//吝汗面倒 规瘤
	if (OtherActor == OwnerCharacter || Cast<ACharacter>(otherActor) == nullptr)
		return;
	if (OwnerCharacter->GetClass() == otherActor->GetClass())
		return;

	if (HittedCharacter.Find(Cast<ACharacter>(otherActor)) != INDEX_NONE)
		return;

	HittedCharacter.AddUnique(Cast<ACharacter>(otherActor));
	
	//公扁面倒 贸府
	if (OnBeginCollision.IsBound())
	{
		FHitResult HitResult;
		Ignore.AddUnique(OwnerCharacter.Get());
		UKismetSystemLibrary::LineTraceSingle(this, Collision->GetComponentLocation(), OtherActor->GetActorLocation(), ETraceTypeQuery::TraceTypeQuery3, false,
			Ignore, EDrawDebugTrace::Type::ForDuration, HitResult, true);
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), HitResult.Location, 10);


		OnBeginCollision.Broadcast(OtherActor, this, HitResult);
	}
}
