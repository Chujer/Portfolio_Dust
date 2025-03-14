// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameObject/Weapon/IdentityObject.h>

#include "CoreMinimal.h"
#include "Action/CDoAction.h"
#include "Engine/DataAsset.h"
#include "GameObject/Weapon/Attachment.h"
#include "WeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class DUST_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

	virtual void BeginDestroy() override;
public:
	UPROPERTY(EditAnywhere, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimInstance;

	UPROPERTY(EditAnywhere, Category = "Attachment")
	TSubclassOf<AAttachment> AttachmentClass;

	UPROPERTY(EditAnywhere, Category = "Identity")
	TSubclassOf<AIdentityObject> IdentityClass;

	UPROPERTY(EditAnywhere, Category = "DoAction")
	TSubclassOf<UCDoAction> DoActionClass;
	
	UPROPERTY(EditAnywhere, Category = "ICon")
	UTexture2D* Icon;
};
