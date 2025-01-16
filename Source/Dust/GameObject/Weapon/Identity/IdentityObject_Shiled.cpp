// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/Weapon/Identity/IdentityObject_Shiled.h"

#include "CLog.h"
#include "Component/StateComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/Character.h"
#include "GameObject/Weapon/Attachment.h"
#include "Kismet/KismetMathLibrary.h"

AIdentityObject_Shiled::AIdentityObject_Shiled()
{
	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	Collision->SetupAttachment(IdentityMesh);
}

void AIdentityObject_Shiled::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AIdentityObject_Shiled::BeginOverlap);
	StateComponent = OwnerCharacter->GetComponentByClass<UStateComponent>();
}

void AIdentityObject_Shiled::BeginIdentity()
{
	Super::BeginIdentity();
	if (GuardAnim == nullptr)
		return;

	if (StateComponent == nullptr)
		return;
	if (!StateComponent->IsIdleMode())
		return;

	StateComponent->SetActionMode();

	OwnerCharacter->PlayAnimMontage(GuardAnim);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AIdentityObject_Shiled::EndIdentity()
{
	Super::EndIdentity();

	StateComponent->SetIdleMode();

	OwnerCharacter->StopAnimMontage();
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AIdentityObject_Shiled::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() == false)
		return;

	AAttachment* OtherAttachment = Cast<AAttachment>(OtherActor);

	if (OtherAttachment == nullptr)
		return;
	FVector start = OtherAttachment->GetActorLocation() - GetActorLocation();
	FVector end = (GetActorLocation() + GetActorForwardVector()) - GetActorLocation();

	CLog::Print(OtherAttachment->GetActorLocation());
	CLog::Print(GetActorLocation());
	CLog::Print(UKismetMathLibrary::Dot_VectorVector(start, end));

	OtherAttachment->AddIgnore(Cast<AActor>(OwnerCharacter));
	OwnerCharacter->LaunchCharacter(FVector(150, 0, 0), false, false);
}
