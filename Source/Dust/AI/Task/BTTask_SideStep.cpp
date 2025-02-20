// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_SideStep.h"

#include "CLog.h"
#include "Character/CEnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_SideStep::UBTTask_SideStep()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_SideStep::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerCharacter = Cast<ACEnemyCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	if (OwnerCharacter == nullptr)
		return EBTNodeResult::Failed;

	isLeft = UKismetMathLibrary::RandomBool();

	if (isLeft == true)
		direction *= -1;

	OwnerCharacter->AddMovementInput(OwnerCharacter->GetActorRightVector() * direction,speed * GetWorld()->GetDeltaSeconds());
	return EBTNodeResult::InProgress;
}

void UBTTask_SideStep::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerCharacter == nullptr)
		return;

	OwnerCharacter->AddMovementInput(OwnerCharacter->GetActorRightVector() * direction, speed * GetWorld()->GetDeltaSeconds());
}
