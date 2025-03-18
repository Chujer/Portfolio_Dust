#include "Component/PlayerSaveComponent.h"

#include "CLog.h"
#include "WeaponComponent.h"
#include "Controller/CLobbyController.h"
#include "Save/CPlayerSaveGame.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


UPlayerSaveComponent::UPlayerSaveComponent()
{
	FilePath = "PlayerWeaponData";
}

void UPlayerSaveComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(UPlayerSaveComponent, SaveGame);
}


void UPlayerSaveComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (FPaths::ValidatePath(FilePath) == false) // 물음표등 잘못된 이름이 존재한다면 FilePath를 SaveDataTemp로 설정
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
	CLog::Print("ReadyLoadSetWeaponData1");
	if (!OwnerCharacter.IsValid()|| SaveGame == nullptr)
		return;

	CLog::Print("ReadyLoadSetWeaponData2");
	//무기 정보 Load 및 해당 무기 장착
	LoadSetWeaponData();
}

void UPlayerSaveComponent::LoadSetWeaponData()
{
	if (!OwnerCharacter.IsValid())
		return;

	CLog::Print("ReadyLoadSetWeaponData3");

	UWeaponComponent* weaponComponent = OwnerCharacter->GetComponentByClass<UWeaponComponent>();
	if (weaponComponent == nullptr)
		return;
	if (SaveGame == nullptr)
		return;

	CLog::Print("StartLoadSetWeaponData");

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
