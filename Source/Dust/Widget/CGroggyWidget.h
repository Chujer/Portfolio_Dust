// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CGroggyWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUST_API UCGroggyWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float GroggyTime = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxGroggyTime = 3.0f;
};
