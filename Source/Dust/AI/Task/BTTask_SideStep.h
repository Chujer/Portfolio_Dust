// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SideStep.generated.h"

/**
 * 
 */
UCLASS()
class DUST_API UBTTask_SideStep : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SideStep();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
public:
	UPROPERTY(EditAnywhere)
	float speed = 10.0f;
	bool isLeft = false;
	float direction = 1;
	class ACEnemyCharacter* OwnerCharacter;
};
