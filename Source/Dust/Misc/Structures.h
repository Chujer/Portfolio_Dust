#pragma once

#include "CoreMinimal.h"
#include "Structures.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText PlayerName = FText::FromString("PlayerName");
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsReady = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TWeakObjectPtr<ACharacter> Character;

};