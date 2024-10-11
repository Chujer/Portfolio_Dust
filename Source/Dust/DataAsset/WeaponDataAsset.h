// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/CDoAction.h"
#include "Engine/DataAsset.h"
#include "GameObject/Weapon/Attachment.h"
#include "Misc/Structures.h"
#include "WeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DUST_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Attachment")
		TSubclassOf<AAttachment> Attachment;

	UPROPERTY(EditAnywhere, Category = "Animation")
		TSubclassOf<UAnimInstance> AnimInstance;

	UPROPERTY(EditAnywhere, Category = "DoActionData")
		TSubclassOf<UCDoAction> DoAction;
};
