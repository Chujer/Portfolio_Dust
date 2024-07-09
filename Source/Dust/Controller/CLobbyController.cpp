#include "Controller/CLobbyController.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "Widget/CLobbyWidget.h"


void ACLobbyController::CreateLobbyWidget_Client_Implementation()
{
	LobbyWidget = Cast<UCLobbyWidget>(CreateWidget(this, LobbyWidgetClass));
	LobbyWidget->AddToViewport();
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
}

void ACLobbyController::UpdatePlayerList_Server_Implementation(const TArray<FPlayerInfo>& PlayerInfos)
{
	ConnectedPlayerInfo = PlayerInfos;
}
