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
	UPROPERTY(EditAnywhere, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimInstance;
public:
	UPROPERTY(EditAnywhere, Category = "Attachment")
	TWeakObjectPtr<class AAttachment> Attachment;

	UPROPERTY(EditAnywhere, Category = "DoAction")
	TWeakObjectPtr<class UCDoAction> DoAction;
	

};
