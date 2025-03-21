// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponDataAsset.h"
#include "UObject/NoExportTypes.h"
#include "WeaponData.generated.h"

/**
 * 
 */
UCLASS()
class DUST_API UWeaponData : public UObject
{
	GENERATED_BODY()

public:
	virtual void BeginDestroy() override;

public:
	UPROPERTY(EditAnywhere, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimInstance;
public:
	UPROPERTY(EditAnywhere, Category = "Attachment")
	class AAttachment* Attachment;

	UPROPERTY(EditAnywhere, Category = "DoAction")
	class UCDoAction* DoAction;
};
