// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/Weapon/Identity/IdentityObject_Shiled.h"

#include "Component/StateComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/Character.h"
#include "GameObject/Weapon/Attachment.h"

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

	OtherAttachment->AddIgnore(Cast<AActor>(OwnerCharacter));
	OwnerCharacter->LaunchCharacter(FVector(100, 0, 0), false, false);
}
