// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Pattern.generated.h"

/**
 * 
 */
UCLASS()
class DUST_API UBTTask_Pattern : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Pattern();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int index = 0;
	class ACEnemyCharacter* OwnerCharacter;
	AActor* target;
	float PatternRange;
};
