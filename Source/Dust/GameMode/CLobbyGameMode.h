#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Misc/Structures.h"
#include "CLobbyGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLastPlayerInGame);

UCLASS()
class DUST_API ACLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	void UpdatePlayerInfo();

	void Test(APawn* OldPawn, APawn* NewPawn);
public:
	TArray<class ACLobbyController*> ConnectedPlayers;
	TArray<FPlayerInfo> ConnectedPlayerInfos;
	
	FLastPlayerInGame OnLastPlayerInGame;
};
