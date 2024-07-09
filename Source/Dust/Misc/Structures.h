#pragma once

#include "CoreMinimal.h"
#include "Structures.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText PlayerName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsReady;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ACharacter* Character;

};