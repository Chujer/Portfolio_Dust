#include "GameObject/Interaction_Object/Interaction_StartNetworkObject.h"

AInteraction_StartNetworkObject::AInteraction_StartNetworkObject()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AInteraction_StartNetworkObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteraction_StartNetworkObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteraction_StartNetworkObject::Interact(ACharacter* player)
{
	OpenNetworkUI(player);
}

void AInteraction_StartNetworkObject::OpenNetworkUI_Implementation(ACharacter* Player)
{
}


