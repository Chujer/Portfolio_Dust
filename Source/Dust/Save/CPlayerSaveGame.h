#pragma once

#include "CoreMinimal.h"
#include "DataAsset/WeaponDataAsset.h"
#include "GameFramework/SaveGame.h"
#include "CPlayerSaveGame.generated.h"

UCLASS()
class DUST_API UCPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()
	

public:
	UPROPERTY(SaveGame)
	int WeaponIndex = 0;
	/*UPROPERTY()
	FTransform PlayerPos;*/
};
