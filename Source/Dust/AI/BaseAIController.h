// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class DUST_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIController();

public:
	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAIPerceptionComponent* AIPerception;
private:
	class UAISenseConfig_Sight* SightConfig;
	class UAISenseConfig_Damage* DamageSenseConfig;
};
