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
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void CreateLobbyWidget_Server();
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void CreateLobbyWidget_NMC();


	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UpdatePlayerList_Server(const TArray<FPlayerInfo>& PlayerInfos);
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void UpdatePlayerList_NMC(const TArray<FPlayerInfo>& PlayerInfos);

	void SetPlayerInfo();

	
	
	UFUNCTION(BlueprintCallable)
	void SetGIPlayerCount(int count);
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
