#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Misc/Structures.h"
#include "CLobbyController.generated.h"

UCLASS()
class DUST_API ACLobbyController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(Client, Unreliable)
	void CreateLobbyWidget_Client();
	UFUNCTION(Reliable, Server)
	void UpdatePlayerList_Server(const TArray<FPlayerInfo>& PlayerInfos);

private:
	//Widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LobbyWidgetClass;
	UPROPERTY()
	TObjectPtr<class UCLobbyWidget> LobbyWidget;

public:
	UPROPERTY()
	FPlayerInfo PlayerInfo;

	UPROPERTY(Replicated)
	TArray<FPlayerInfo> ConnectedPlayerInfo;
};
