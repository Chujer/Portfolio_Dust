// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_LookAt.h"

#include "Character/CEnemyCharacter.h"

EBTNodeResult::Type UBTTask_LookAt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACEnemyCharacter* OwnerCharacter = Cast<ACEnemyCharacter>(OwnerComp.GetAIOwner()->GetCharacter());


	AActor* target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
	if (OwnerCharacter == nullptr || target == nullptr)
		return EBTNodeResult::Failed;
	//�ִϸ��̼��� �������϶��� �������� ����
	if (OwnerCharacter->GetCurrentMontage() != nullptr)
		return EBTNodeResult::Failed;

	OwnerCharacter->LookAtLERP(target);

	return EBTNodeResult::Succeeded;
}
