#include "GameMode/CLobbyGameMode.h"

#include "CLog.h"
#include "Controller/CLobbyController.h"
#include "GameInstance/CGameInstance.h"

void ACLobbyGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ACLobbyController* newPlayerController = Cast<ACLobbyController>(NewPlayer);

	if(newPlayerController == nullptr)
	{
		return;
	}

	newPlayerController->SetPlayerInfo();

	ConnectedPlayers.Add(newPlayerController);
	ConnectedPlayerInfos.Add(newPlayerController->PlayerInfo);

	for (ACLobbyController* connectedPlayer : ConnectedPlayers)
	{
		connectedPlayer->UpdatePlayerList(ConnectedPlayerInfos);
	}

	
}