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

USTRUCT()
struct FDoActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;
	UPROPERTY(EditAnywhere)
		bool bCanMove = false;

public:
	UPROPERTY(EditAnywhere)
		float Power;
	UPROPERTY(EditAnywhere)
		FVector Launch = FVector(100.0f, 0, 0);
	UPROPERTY(EditAnywhere)
		float StopTime;
	UPROPERTY(EditAnywhere)
		USoundWave* HittingSound;
	UPROPERTY(EditAnywhere)
		bool bWithLaunch = false;

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<UCameraShakeBase> CameraShakeClass;
};