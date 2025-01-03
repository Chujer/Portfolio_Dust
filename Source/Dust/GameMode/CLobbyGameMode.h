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

	void Test(APawn* OldPawn, APawn* NewPawn);
public:
	TArray<class ACLobbyController*> ConnectedPlayers;
	TArray<FPlayerInfo> ConnectedPlayerInfos;
	
	
};
