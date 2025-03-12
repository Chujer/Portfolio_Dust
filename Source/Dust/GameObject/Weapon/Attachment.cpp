#include "GameObject/Weapon/Attachment.h"

#include <filesystem>

#include "CLog.h"
#include "Action/CDoAction.h"
#include "Character/CBaseCharacter.h"
#include "Component/StateComponent.h"
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

void AAttachment::SetCollision_Implementation(ECollisionEnabled::Type value)
{
	Collision->SetCollisionEnabled(value);
}

void AAttachment::AddIgnore_Implementation(AActor* Actor)
{
	if (ACBaseCharacter* character = Cast<ACBaseCharacter>(Actor))
	{
		HittedCharacter.AddUnique(character);
	}
}

void AAttachment::ClearHittedCharacter_Implementation()
{
	HittedCharacter.Empty();
	invincibilityCharacter.Empty();
}

void AAttachment::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACBaseCharacter>(GetOwner());
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
	ACBaseCharacter* otherActor = Cast<ACBaseCharacter>(OtherActor);

	//�ߺ��浹 ����
	if (OtherActor == OwnerCharacter || Cast<ACBaseCharacter>(otherActor) == nullptr)
		return;
	if (OwnerCharacter->GetClass() == otherActor->GetClass())
		return;

	//���������� ĳ����
	if (UStateComponent* otherState = otherActor->GetComponentByClass<UStateComponent>())
	{
		if (otherState->IsRollMode() || otherState->IsDownMode() || otherState->IsExecuteMode())
			invincibilityCharacter.AddUnique(otherActor);
	}

	if (HittedCharacter.Find(Cast<ACBaseCharacter>(otherActor)) != INDEX_NONE || invincibilityCharacter.Find(Cast<ACBaseCharacter>(otherActor)) != INDEX_NONE)
		return;

	//�̹� Ÿ���� �� �߰�
	HittedCharacter.AddUnique(Cast<ACBaseCharacter>(otherActor));
	
	//�����浹 ó��
	if (OnBeginCollision.IsBound())
	{
		FHitResult HitResult;
		TArray<AActor*> Ignore;
		Ignore.AddUnique(OwnerCharacter.Get());
		UKismetSystemLibrary::LineTraceSingle(this, Collision->GetComponentLocation(), OtherActor->GetActorLocation(), ETraceTypeQuery::TraceTypeQuery3, false,
			Ignore, EDrawDebugTrace::Type::None, HitResult, true);

		//Hit����
		UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
		OwnerCharacter->PlaySoundRPC(HitSound);

		OnBeginCollision.Broadcast(OtherActor, this, HitResult, isNormalDamage);
	}
}
