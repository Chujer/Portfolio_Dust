// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class DUST_API IInteractionInterface
{
	GENERATED_BODY()
	
public:
	virtual void Interact(ACharacter* player) = 0;
};
