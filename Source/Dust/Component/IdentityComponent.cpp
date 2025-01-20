#include "Component/IdentityComponent.h"

#include "CLog.h"
#include "GameObject/Weapon/IdentityObject.h"
#include "Net/UnrealNetwork.h"

UIdentityComponent::UIdentityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UIdentityComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
}

void UIdentityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UIdentityComponent, Identity);
}

void UIdentityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UIdentityComponent::BeginIdentity()
{
	IsIdentityState = true;
	Identity->BeginIdentity();
}

void UIdentityComponent::EndIdentity()
{
	IsIdentityState = false;
	Identity->EndIdentity();
}

void UIdentityComponent::BeginIdentitySkill()
{
	if(IsIdentityState == true)
		Identity->BeginIdentitySkill();
}

void UIdentityComponent::EndIdentitySkill()
{
	Identity->EndIdentitySkill();
}

void UIdentityComponent::SetIdentity_Implementation(TSubclassOf<AIdentityObject> IdentityClass)
{
	if (IdentityClass == nullptr)
		return;

	if (Identity != nullptr)
		Identity->Destroy();

	FActorSpawnParameters param;
	param.Owner = Cast<AActor>(OwnerCharacter);
	Identity = GetWorld()->SpawnActor<AIdentityObject>(IdentityClass,param);
}
