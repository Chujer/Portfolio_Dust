#include "Component/PlayerSaveComponent.h"

#include "CLog.h"
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

	FilePath += temp->PlayerInfo.PlayerName.ToString();

	if (FPaths::ValidatePath(FilePath) == false)
		FilePath = "SaveDataTemp";

	if (UGameplayStatics::DoesSaveGameExist(FilePath, 0) == true)	//해당경로에 데이터가 있다면
	{
		SaveGame = Cast<UCPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(FilePath, 0));
	}
	else
	{
		SaveGame = Cast<UCPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	}
	
	
}

void UPlayerSaveComponent::LoadData()
{
	if (!OwnerCharacter.IsValid()|| SaveGame == nullptr)
		return;

	//무기 정보 Load 및 해당 무기 장착
	LoadSetWeaponData();
}

void UPlayerSaveComponent::LoadSetWeaponData()
{
	if (!OwnerCharacter.IsValid())
		return;

	UWeaponComponent* weaponComponent = OwnerCharacter->GetComponentByClass<UWeaponComponent>();
	if (weaponComponent == nullptr)
		return;
	CLog::Print("HaveWeaponComp");
	if (SaveGame == nullptr)
		return;
	CLog::Print("HaveSaveGame");

	weaponComponent->SetWeaponData_Server(SaveGame->WeaponIndex);
}

void UPlayerSaveComponent::SaveData()
{
	if (OwnerCharacter.IsValid() && SaveGame == nullptr)
		return;

	UGameplayStatics::SaveGameToSlot(SaveGame, FilePath, 0);

	if (UGameplayStatics::DoesSaveGameExist(FilePath, 0) == true)		//해당경로에 데이터가 있다면
	{
		SaveGame->WeaponIndex = OwnerCharacter->GetComponentByClass<UWeaponComponent>()->curWeaponIndex;
		UGameplayStatics::SaveGameToSlot(SaveGame, FilePath, 0);
	}
	else
	{ 
		SaveGame = Cast<UCPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
		SaveData();
	}
}
