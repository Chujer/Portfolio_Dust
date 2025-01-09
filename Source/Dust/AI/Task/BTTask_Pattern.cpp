// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Pattern.h"

#include "AIController.h"
#include "Character/CEnemyCharacter.h"
#include "Component/WeaponComponent.h"

EBTNodeResult::Type UBTTask_Pattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACEnemyCharacter* OwnerCharacter = Cast<ACEnemyCharacter>(OwnerComp.GetAIOwner()->GetCharacter());

	if (OwnerCharacter == nullptr)
		return EBTNodeResult::Failed;

	OwnerCharacter->WeaponComponent->DoIndexAction_Server(index);
	

	return EBTNodeResult::Succeeded;
}
