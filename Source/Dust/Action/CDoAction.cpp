// Fill out your copyright notice in the Description page of Project Settings.


#include "CDoAction.h"
#include "CLog.h"
#include "Component/MoveComponent.h"
#include "Component/StateComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/CBaseCharacter.h"
#include "Character/CEnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"

UCDoAction::UCDoAction()
{
	MaxActionIndex = DoActionDatas.Num();
}
void UCDoAction::BeginPlay(ACBaseCharacter* InOwner)
{
	OwnerCharacter = InOwner;
	if (OwnerCharacter.IsValid())
	{
		StateComponent = OwnerCharacter->GetComponentByClass<UStateComponent>();
		MoveComponent = OwnerCharacter->GetComponentByClass<UMoveComponent>();
	}

}

void UCDoAction::DoActionTrigger()
{
}

void UCDoAction::DoAction_Server()
{
	if (!StateComponent.IsValid())
		return;

	StateComponent->SetActionMode();
}

void UCDoAction::DoAction_NMC()
{
	if (MoveComponent.IsValid())
	{
		//��Ÿ�� ����� ĳ���� �̵��Ұ� ����
		MoveComponent->SetStop(!DoActionDatas[ActionIndex].bCanMove);
	}
	if (OwnerCharacter.IsValid() && !!DoActionDatas[ActionIndex].Montage)
	{
		OwnerCharacter->PlayAnimMontage(DoActionDatas[ActionIndex].Montage, DoActionDatas[ActionIndex].PlayRate);
		currentDoActionData = DoActionDatas[ActionIndex];
	}
}

void UCDoAction::DoIndexAction_NMC(int Index)
{
	if (OwnerCharacter.IsValid() && !!DoActionDatas[Index].Montage)
	{
		OwnerCharacter->PlayAnimMontage(DoActionDatas[Index].Montage, DoActionDatas[Index].PlayRate);
		currentDoActionData = DoActionDatas[Index];
	}
}

void UCDoAction::EndDoAtion_Server()
{
	if (StateComponent != nullptr )
		StateComponent->SetIdleMode();

	if (MoveComponent != nullptr)
		MoveComponent->SetStop(false);
}

void UCDoAction::EndDoAtion_NMC()
{
	ActionIndex = 0;
}

void UCDoAction::DoActionRight_Server()
{
	if (!StateComponent.IsValid())
		return;
	
	if (StateComponent->IsIdleMode())
	{
		ACEnemyCharacter* enemy = Cast<ACEnemyCharacter>(SearchCanExecut());
		if (enemy != nullptr)
		{
			FVector vector1 = enemy->GetActorForwardVector();
			FVector vector2 = OwnerCharacter->GetActorForwardVector();

			float scala = UKismetMathLibrary::Dot_VectorVector(vector1, vector2);
			CLog::Print(scala);
			if (scala < -0.9)
				Execute(enemy);
		}
	}

}

void UCDoAction::DoActionRight_NMC()
{
}

//ó�� �Ǻ�
AActor* UCDoAction::SearchCanExecut()
{
	FVector start = OwnerCharacter->GetActorLocation();
	FVector end = start + OwnerCharacter->GetActorForwardVector() * 100;
	TArray<AActor*> ignore;
	ignore.Add(Cast<AActor>(OwnerCharacter));
	FHitResult hitResult;

	bool result = UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery3, false,
		ignore, EDrawDebugTrace::Type::ForDuration, hitResult, true);

	if (result == false)
		return nullptr;

	if (!hitResult.GetActor()->GetComponentByClass<UStateComponent>()->IsGroggyMode())
		return nullptr;

	return hitResult.GetActor();
}

//ó��
void UCDoAction::Execute(class ACEnemyCharacter* Target)
{
	ExecuteTarget = Target;
	MoveComponent->SetCamerafix(true);

	OwnerCharacter->PlayMontage_Server(ExecuteAnimData.ExecuteAnimation_Executioner);
	Target->PlayMontage_Server(ExecuteAnimData.ExecuteAnimation_Target);

	//ó�� �� ����
	Target->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Target->GetActorLocation(), OwnerCharacter->GetActorLocation()));
	OwnerCharacter->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), Target->GetActorLocation()));

	StateComponent->SetExecuteMode();
	Target->StateComponent->SetExecuteMode();
}

void UCDoAction::LaunchCharacter(FDoActionData DoActionData, ACharacter* LaunchCharacter)
{
	if (!OwnerCharacter.IsValid() || !LaunchCharacter)
		return;

	FVector launchVector = OwnerCharacter->GetActorForwardVector() * DoActionData.Launch.X;
	launchVector.Z = DoActionData.Launch.Z;

	//ĳ���� ��ġ
	LaunchCharacter->LaunchCharacter(launchVector, true, true);
	//�����ڵ� �Բ� ��ġ
	if (DoActionData.bWithLaunch)
		OwnerCharacter->LaunchCharacter(launchVector, true, true);
}

void UCDoAction::ApplyDamage(AActor* OtherActor, class AAttachment* Attachment, const FHitResult& HitResult)
{
	//UKismetSystemLibrary::DrawDebugSphere(this, HitResult.Location, 10, 12, FLinearColor::White, 5);
	SpawnHitEffect(HitResult.Location);
	
	UStateComponent* stateComponent = OtherActor->GetComponentByClass<UStateComponent>();
	if (stateComponent == nullptr)
		return;

	if (OwnerCharacter->HasAuthority())
		stateComponent->SubHP(currentDoActionData.Power);
}

void UCDoAction::SpawnHitEffect(FVector Location)
{
	if (currentDoActionData.HittEffect == nullptr)
		return;
	//�ǰ� ����Ʈ ����
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(OwnerCharacter->GetWorld(), currentDoActionData.HittEffect, Location);
}
