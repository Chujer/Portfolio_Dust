#include "Character/CPlayerCharacter.h"

#include "Utility/CHelpers.h"


ACPlayerCharacter::ACPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	//CHelpers::CreateComponent<UMoveComponent>(this, &MoveComponent, "MoveComponent");
	MoveComponent = CreateDefaultSubobject<UMoveComponent>("MoveComponent");

}

void ACPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

