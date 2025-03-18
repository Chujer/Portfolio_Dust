#include "Controller/CLobbyController.h"

#include "AdvancedSteamFriendsLibrary.h"
#include "AdvancedSessionsLibrary.h"
#include "CLog.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameInstance/CGameInstance.h"
#include "GameMode/CLobbyGameMode.h"
#include "Widget/CLobbyWidget.h"


void ACLobbyController::BeginPlay()
{
	Super::BeginPlay();
	ConsoleCommand(TEXT("DisableAllScreenMessages"));
}

void ACLobbyController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACLobbyController, ConnectedPlayerInfo);
	DOREPLIFETIME(ACLobbyController, PlayerInfo);
}

void ACLobbyController::CreateLobbyWidget_Server_Implementation()
{
	CreateLobbyWidget_NMC();
}

void ACLobbyController::CreateLobbyWidget_NMC_Implementation()
{
	if (!IsLocalPlayerController())
		return;
	LobbyWidget = Cast<UCLobbyWidget>(CreateWidget(this, LobbyWidgetClass));


	LobbyWidget->AddToViewport();
	if (!!LobbyWidget)
		LobbyWidget->UpdatePlayerList(ConnectedPlayerInfo);
}

void ACLobbyController::SetPlayerInfo()
{
	UAdvancedSessionsLibrary::GetUniqueNetID(this, UNetID);
	if (!UNetID.IsValid())
		return;
	PlayerInfo.PlayerName = FText::FromString(UAdvancedSteamFriendsLibrary::GetSteamPersonaName(UNetID));
	PlayerInfo.PController = this;
}

void ACLobbyController::SetGIPlayerCount(int count)
{
	//미션에 참가한 총 플레이어 수
	for (FPlayerInfo connectPlayerInfo : ConnectedPlayerInfo)
	{
		if (connectPlayerInfo.PController == nullptr)
			continue;
		if (UCGameInstance* instance = Cast<UCGameInstance>(connectPlayerInfo.PController->GetGameInstance()))
		{
			instance->MissionPlayerCount = count;
		}
	}
}

void ACLobbyController::UpdatePlayerList_Server_Implementation(const TArray<FPlayerInfo>& PlayerInfos)
{
	UpdatePlayerList_NMC(PlayerInfos);
}

void ACLobbyController::UpdatePlayerList_NMC_Implementation(const TArray<FPlayerInfo>& PlayerInfos)
{
	ConnectedPlayerInfo = PlayerInfos;

	if (!!LobbyWidget)
		LobbyWidget->UpdatePlayerList(ConnectedPlayerInfo);
}