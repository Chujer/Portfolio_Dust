#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CGameInstance.generated.h"

UCLASS()
class DUST_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MissionPlayerCount = 99;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MouseSenceX = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MouseSenceY = 1.0f;
};
