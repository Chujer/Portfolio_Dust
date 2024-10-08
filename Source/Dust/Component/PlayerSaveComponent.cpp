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
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	ACLobbyController* temp = Cast<ACLobbyController>(OwnerCharacter->GetController());

	if (temp == nullptr)
		return;
	FilePath += Cast<ACLobbyController>(OwnerCharacter->GetController())->PlayerInfo.PlayerName.ToString();

	if (UGameplayStatics::DoesSaveGameExist(FilePath, 0) == true)		//�ش��ο� �����Ͱ� �ִٸ�
	{
		SaveGame = Cast<UCPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(FilePath, 0));
	}
	else
	{
		//���̺� ���� ����
		SaveGame = Cast<UCPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	}
	
}

void UPlayerSaveComponent::LoadData()
{
	if (OwnerCharacter.IsValid() || SaveGame == nullptr)
		return;

	OwnerCharacter->SetActorTransform(SaveGame->PlayerPos);
}

void UPlayerSaveComponent::SaveData(FTransform data)
{
	if (OwnerCharacter.IsValid() && SaveGame != nullptr)
		return;

	UGameplayStatics::SaveGameToSlot(SaveGame, FilePath, 0);

	//if (UGameplayStatics::DoesSaveGameExist(FilePath, 0) == true)		//�ش��ο� �����Ͱ� �ִٸ�
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
