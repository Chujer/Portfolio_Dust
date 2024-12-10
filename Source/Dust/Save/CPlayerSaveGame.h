#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CPlayerSaveGame.generated.h"

UCLASS()
class DUST_API UCPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()
	

public:
	class UWeaponDataAsset* WeaponDataAsset;
};
