// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUST_API UCHPWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	FORCEINLINE void SubHP(float Damage) { HP -= Damage; }

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxHP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HP;
};
