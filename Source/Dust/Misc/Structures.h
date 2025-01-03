#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
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
	TWeakObjectPtr<class ACLobbyController> PController;

};

USTRUCT()
struct FDoActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		UAnimMontage* Montage = nullptr;
	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;
	UPROPERTY(EditAnywhere)
		bool bCanMove = false;

public:
	UPROPERTY(EditAnywhere)
		float Power = 1.0f;
	UPROPERTY(EditAnywhere)
		FVector Launch = FVector(100.0f, 0, 0);
	UPROPERTY(EditAnywhere)
	float StopTime = 0.0f;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* HittEffect = nullptr;
	UPROPERTY(EditAnywhere)
		USoundWave* HittingSound = nullptr;
	UPROPERTY(EditAnywhere)
		bool bWithLaunch = false;

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<UCameraShakeBase> CameraShakeClass;
};

USTRUCT()
struct FEvadeData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage = nullptr;
	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;
	
};

USTRUCT(BlueprintType)

struct FWeaponDataStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaponName = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWeaponDataAsset* WeaponDataAsset = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ICon = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int WeaponNum = 0;
};