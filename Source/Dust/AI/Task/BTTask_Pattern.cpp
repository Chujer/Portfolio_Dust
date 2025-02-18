// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Pattern.h"

#include "AIController.h"
#include "CLog.h"
#include "Action/CDoAction.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CEnemyCharacter.h"
#include "Component/StateComponent.h"
#include "Component/WeaponComponent.h"

UBTTask_Pattern::UBTTask_Pattern()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Pattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	OwnerCharacter = Cast<ACEnemyCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
	if (OwnerCharacter == nullptr || target == nullptr)
		return EBTNodeResult::Failed;
	if (OwnerCharacter->GetComponentByClass<UWeaponComponent>()->GetDoAction() == nullptr)
		return EBTNodeResult::Failed;


	index = OwnerComp.GetBlackboardComponent()->GetValueAsInt("PatternIndex");
	float distance = OwnerCharacter->GetDistanceTo(target);

	PatternRange = OwnerCharacter->GetComponentByClass<UWeaponComponent>()->GetDoAction()->DoActionDatas[index].Range;
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("Distance", distance);


	if (distance > PatternRange)
	{
		//패턴의 사정거리에 닿지 않는 경우 InProgress상태로 만들어 Move To를 실행
		return EBTNodeResult::InProgress;
	}

	OwnerCharacter->LookAtTarget(target);
	OwnerCharacter->WeaponComponent->DoIndexAction_Server(index);

	return EBTNodeResult::Succeeded;
}

void UBTTask_Pattern::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if(OwnerCharacter->GetComponentByClass<UStateComponent>()->IsIdleMode() == false)
	{
		return;
	}

	float distance = OwnerCharacter->GetDistanceTo(target);

	PatternRange = OwnerCharacter->GetComponentByClass<UWeaponComponent>()->GetDoAction()->DoActionDatas[index].Range;
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("Distance", distance);


	if (distance > PatternRange)
		return;

	OwnerCharacter->LookAtTarget(target);
	OwnerCharacter->WeaponComponent->DoIndexAction_Server(index);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
