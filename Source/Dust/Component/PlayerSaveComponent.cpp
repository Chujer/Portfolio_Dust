#include "Component/PlayerSaveComponent.h"
#include "Save/CPlayerSaveGame.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UPlayerSaveComponent::UPlayerSaveComponent()
{
	FilePath = "SaveData";
}


void UPlayerSaveComponent::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<ACharacter>(GetOwner());
	
}

void UPlayerSaveComponent::LoadData()
{
	if (PlayerCharacter.IsValid())
		return;
	SaveGame = Cast<UCPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(FilePath, 0));

	PlayerCharacter->SetActorTransform(SaveGame->PlayerPos);
}

void UPlayerSaveComponent::SaveData(FTransform data)
{
	if (PlayerCharacter.IsValid())
		return;

	if (UGameplayStatics::DoesSaveGameExist(FilePath, 0) == true)
	{
		SaveGame->PlayerPos = PlayerCharacter->GetTransform();
		UGameplayStatics::SaveGameToSlot(SaveGame, FilePath, 0);
	}
	else
	{
		SaveGame = Cast<UCPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
		SaveData(data);
	}
}
