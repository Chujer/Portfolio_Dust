// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/Weapon/Identity/IdentityObject_Shiled.h"

#include "CLog.h"
#include "Character/CEnemyCharacter.h"
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

	PlayMontage_Server(GuardAnim);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AIdentityObject_Shiled::EndIdentity()
{
	Super::EndIdentity();


	if (IsParrying == true)
		return;
	StateComponent->SetIdleMode();
	OwnerCharacter->StopAnimMontage();
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AIdentityObject_Shiled::BeginIdentitySkill()
{
	Super::BeginIdentitySkill();
	if (ParryingAnim == nullptr)
		return;
	if (StateComponent == nullptr)
		return;

	IsParrying = true;

	StateComponent->SetActionMode();

	PlayMontage_Server(ParryingAnim);
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AIdentityObject_Shiled::EndIdentitySkill()
{
	Super::EndIdentitySkill();
	IsParrying = false;
	StateComponent->SetIdleMode();
}


void AIdentityObject_Shiled::GuardOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAttachment* OtherAttachment = Cast<AAttachment>(OtherActor);
	if (OtherAttachment == nullptr)
		return;

	FVector vector1 = OtherAttachment->GetActorLocation() - GetActorLocation();
	FVector vector2 = (GetActorLocation() + OwnerCharacter->GetActorForwardVector()) - GetActorLocation();

	float scala = UKismetMathLibrary::Dot_VectorVector(vector1, vector2);
	if (scala < 20)
		return;
	OtherAttachment->AddIgnore(Cast<AActor>(OwnerCharacter));
	OwnerCharacter->LaunchCharacter(OwnerCharacter->GetActorForwardVector() * -150.f, false, false);
}

void AIdentityObject_Shiled::ParryOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACEnemyCharacter* enemy = Cast<ACEnemyCharacter>(OtherActor->Owner);
	if (enemy == nullptr)
		return;
	UStateComponent* enemyStateComponent = enemy->GetComponentByClass<UStateComponent>();
	if (enemyStateComponent == nullptr)
		return;
	
	enemyStateComponent->SetHittingParryMode();

}


void AIdentityObject_Shiled::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() == false)
		return;

	if (IsParrying == true)
	{
		ParryOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
	else
		GuardOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
