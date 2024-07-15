#include "Controller/CLobbyController.h"

#include "AdvancedSteamFriendsLibrary.h"
#include "AdvancedSessionsLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "Widget/CLobbyWidget.h"


void ACLobbyController::CreateLobbyWidget_Client_Implementation()
{
	LobbyWidget = Cast<UCLobbyWidget>(CreateWidget(this, LobbyWidgetClass));
	
	LobbyWidget->AddToViewport();
	if(!!LobbyWidget)
		LobbyWidget->UpdatePlayerList(ConnectedPlayerInfo);
}

void ACLobbyController::BeginPlay()
{
	Super::BeginPlay();
	CreateLobbyWidget_Client();
}

void ACLobbyController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACLobbyController, ConnectedPlayerInfo);
	DOREPLIFETIME(ACLobbyController, PlayerInfo);
}

void ACLobbyController::SetPlayerInfo()
{
	UAdvancedSessionsLibrary::GetUniqueNetID(this, UNetID);
	PlayerInfo.PlayerName = FText::FromString(UAdvancedSteamFriendsLibrary::GetSteamPersonaName(UNetID));
	//PlayerInfo.Character = Cast<ACharacter>(GetPawn());
	//PlayerInfo.PlayerName = FText::FromString(Player.GetName());
}

void ACLobbyController::UpdatePlayerList(const TArray<FPlayerInfo>& PlayerInfos)
{
	ConnectedPlayerInfo = PlayerInfos;

	if (!!LobbyWidget)
		LobbyWidget->UpdatePlayerList(ConnectedPlayerInfo);
}
