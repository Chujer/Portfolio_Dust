#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Misc/Structures.h"
#include "CLobbyGameMode.generated.h"

UCLASS()
class DUST_API ACLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

public:
	TArray<class ACLobbyController*> ConnectedPlayers;
	TArray<FPlayerInfo> ConnectedPlayerInfos;
};
