#include "Component/PlayerSaveComponent.h"

#include "Controller/CLobbyController.h"
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

	ACLobbyController* temp = Cast<ACLobbyController>(PlayerCharacter->GetController());

	if (temp == nullptr)
		return;
	FilePath += Cast<ACLobbyController>(PlayerCharacter->GetController())->PlayerInfo.PlayerName.ToString();

	if (UGameplayStatics::DoesSaveGameExist(FilePath, 0) == true)		//해당경로에 데이터가 있다면
	{
		SaveGame = Cast<UCPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(FilePath, 0));
	}
	else
	{
		//세이브 게임 제작
		SaveGame = Cast<UCPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	}
	
}

void UPlayerSaveComponent::LoadData()
{
	if (PlayerCharacter.IsValid() || SaveGame == nullptr)
		return;

	PlayerCharacter->SetActorTransform(SaveGame->PlayerPos);
}

void UPlayerSaveComponent::SaveData(FTransform data)
{
	if (PlayerCharacter.IsValid() && SaveGame != nullptr)
		return;

	UGameplayStatics::SaveGameToSlot(SaveGame, FilePath, 0);

	//if (UGameplayStatics::DoesSaveGameExist(FilePath, 0) == true)		//해당경로에 데이터가 있다면
	//{
	//	SaveGame->PlayerPos = PlayerCharacter->GetTransform();
	//	UGameplayStatics::SaveGameToSlot(SaveGame, FilePath, 0);
	//}
	//else
	//{
	//	SaveGame = Cast<UCPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	//	SaveData(data);
	//}
}
