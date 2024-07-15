#pragma once

#include "CoreMinimal.h"
#include "BlueprintDataDefinitions.h"
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
	UFUNCTION(Client, Reliable)
	void CreateLobbyWidget_Client();
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerList(const TArray<FPlayerInfo>& PlayerInfos);

	void SetPlayerInfo();


private:
	//Widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LobbyWidgetClass;
	UPROPERTY()
	TObjectPtr<class UCLobbyWidget> LobbyWidget;

public:
	UPROPERTY(BlueprintReadWrite, Replicated)
	FPlayerInfo PlayerInfo;

	UPROPERTY(BlueprintReadWrite, Replicated)
	TArray<FPlayerInfo> ConnectedPlayerInfo;


	FBPUniqueNetId UNetID;
};
