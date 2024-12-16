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
	if (!UNetID.IsValid())
		return;
	PlayerInfo.PlayerName = FText::FromString(UAdvancedSteamFriendsLibrary::GetSteamPersonaName(UNetID));
	PlayerInfo.PController = this;
}

void ACLobbyController::UpdatePlayerList(const TArray<FPlayerInfo>& PlayerInfos)
{
	ConnectedPlayerInfo = PlayerInfos;

	if (!!LobbyWidget)
		LobbyWidget->UpdatePlayerList(ConnectedPlayerInfo);
}
