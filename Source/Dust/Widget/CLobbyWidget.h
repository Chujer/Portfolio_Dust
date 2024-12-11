#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Misc/Structures.h"
#include "CLobbyWidget.generated.h"

UCLASS()
class DUST_API UCLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdatePlayerList(const TArray<FPlayerInfo>& PlayerInfos);

	void UpdatePlayerList_Implementation(const TArray<FPlayerInfo>& PlayerInfos);
};

