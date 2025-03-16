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
	UpdatePlayerInfo();
}

void ACLobbyGameMode::UpdatePlayerInfo()
{
	ConnectedPlayerInfos.Empty();
	// 서버에 접속한 플레이어 추가
	for (ACLobbyController* connectedPlayer : ConnectedPlayers)
	{
		ConnectedPlayerInfos.Add(connectedPlayer->PlayerInfo);

	}
	//각 클라이언트의 컨트롤러에 접속한 플레이어 정보 갱신
	for (ACLobbyController* connectedPlayer : ConnectedPlayers)
	{
		connectedPlayer->UpdatePlayerList_Server(ConnectedPlayerInfos);
	}
}
