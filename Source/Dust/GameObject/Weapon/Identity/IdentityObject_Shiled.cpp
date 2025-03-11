// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/Weapon/Identity/IdentityObject_Shiled.h"

#include "CLog.h"
#include "Character/CEnemyCharacter.h"
#include "Component/MoveComponent.h"
#include "Component/StateComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/Character.h"
#include "GameObject/Weapon/Attachment.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

AIdentityObject_Shiled::AIdentityObject_Shiled()
{
	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	Collision->SetupAttachment(IdentityMesh);
	Collision->SetIsReplicated(true);
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

	OwnerCharacter->PlayMontage_Server(GuardAnim);
	SetCollision(ECollisionEnabled::QueryOnly);
}

void AIdentityObject_Shiled::EndIdentity()
{
	Super::EndIdentity();


	if (IsParrying == true)
		return;
	StateComponent->SetIdleMode();

	if (OwnerCharacter->GetCurrentMontage() == GuardAnim)
		OwnerCharacter->StopMontage_Server();


	SetCollision(ECollisionEnabled::NoCollision);
}

void AIdentityObject_Shiled::BeginIdentitySkill()
{
	Super::BeginIdentitySkill();
	if (ParryingAnim == nullptr)
		return;
	if (StateComponent == nullptr)
		return;
	if (StateComponent->IsDownMode())
		return;

	SetParry(true);

	StateComponent->SetActionMode();

	OwnerCharacter->PlayMontage_Server(ParryingAnim);
	OwnerCharacter->GetComponentByClass<UMoveComponent>()->SetStop(true);

	SetCollision(ECollisionEnabled::NoCollision);
	if (HasAuthority())
		CLog::Print(IsParrying);

}

void AIdentityObject_Shiled::EndIdentitySkill()
{
	Super::EndIdentitySkill();
	SetParry(false);
	StateComponent->SetIdleMode();
}

void AIdentityObject_Shiled::SetParry_Implementation(bool isParry)
{
	IsParrying = isParry;
}

void AIdentityObject_Shiled::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AIdentityObject_Shiled, IsParrying);
}


void AIdentityObject_Shiled::GuardOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAttachment* OtherAttachment = Cast<AAttachment>(OtherActor);
	if (OtherAttachment == nullptr)
		return;

	//FowardVector를 사용하지 않은이유 : 해당 가드의 경우 기준점은 적이 바라보는 방향이 아닌 적 무기의 위치에 따라 판별해야 하기 때문
	FVector vector1 = OtherAttachment->GetActorLocation() - GetActorLocation();
	FVector vector2 = (GetActorLocation() + OwnerCharacter->GetActorForwardVector()) - GetActorLocation();
	
	float scala = UKismetMathLibrary::Dot_VectorVector(vector1, vector2);
	if (scala < 20)
		return;
	//충돌 제외 캐릭터에 추가
	OtherAttachment->AddIgnore(Cast<AActor>(OwnerCharacter));

	if(BlockAnim != nullptr)
		OwnerCharacter->PlayMontage_Server(BlockAnim);

	UGameplayStatics::PlaySound2D(GetWorld(), GaurdSound);

	OwnerCharacter->LaunchCharacter(OwnerCharacter->GetActorForwardVector() * -150, false, false);

	if (CameraShakeClass != nullptr)
		Cast<APlayerController>(OwnerCharacter->GetController())->PlayerCameraManager->StartCameraShake(CameraShakeClass);
}

void AIdentityObject_Shiled::ParryOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACEnemyCharacter* enemy = Cast<ACEnemyCharacter>(OtherActor->Owner);
	AAttachment* OtherAttachment = Cast<AAttachment>(OtherActor);
	if (enemy == nullptr)
		return;
	UStateComponent* enemyStateComponent = enemy->GetComponentByClass<UStateComponent>();
	if (enemyStateComponent == nullptr)
		return;

	//충돌 제외 캐릭터에 추가
	OtherAttachment->AddIgnore(Cast<AActor>(OwnerCharacter));
	UGameplayStatics::PlaySound2D(GetWorld(), ParrySound);
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
