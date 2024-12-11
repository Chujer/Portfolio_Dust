#include "Component/PlayerSaveComponent.h"

#include "WeaponComponent.h"
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
	
	//FilePath += Cast<ACLobbyController>(OwnerCharacter->GetController())->PlayerInfo.PlayerName.ToString();
	FilePath += OwnerCharacter->GetName();
	
	SaveGame = Cast<UCPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	
	if (UGameplayStatics::DoesSaveGameExist(FilePath, 0) == true)	//해당경로에 데이터가 있다면
	{
		SaveGame = Cast<UCPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(FilePath, 0));
		return;
	}
	//else
	//{
	//	SaveGame = Cast<UCPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	//	//세이브 게임 제작
	//	//SaveGame = Cast<UCPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	//}
	
}

void UPlayerSaveComponent::LoadData()
{
	if (OwnerCharacter.IsValid() || SaveGame == nullptr)
		return;

	OwnerCharacter->GetComponentByClass<UWeaponComponent>()->SetWeaponData_Server(SaveGame->WeaponDataAsset);
}

void UPlayerSaveComponent::SaveData(UWeaponDataAsset* data)
{
	int a = 0;

	if (OwnerCharacter.IsValid() && SaveGame == nullptr)
		return;

	UGameplayStatics::SaveGameToSlot(SaveGame, FilePath, 0);

	if (UGameplayStatics::DoesSaveGameExist(FilePath, 0) == true)		//해당경로에 데이터가 있다면
	{
		SaveGame->WeaponDataAsset = OwnerCharacter->GetComponentByClass<UWeaponComponent>()->GetWeaponDataAsset();
		UGameplayStatics::SaveGameToSlot(SaveGame, FilePath, 0);
	}
	else
	{
		SaveGame = Cast<UCPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
		SaveData(data);
	}
}
