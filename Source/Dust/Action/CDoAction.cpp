// Fill out your copyright notice in the Description page of Project Settings.


#include "CDoAction.h"

#include "AIController.h"
#include "CLog.h"
#include "Component/MoveComponent.h"
#include "Component/StateComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CBaseCharacter.h"
#include "Character/CEnemyCharacter.h"
#include "Character/CPlayerCharacter.h"
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
		//몽타주 재생중 캐릭터 이동불가 설정
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
		ActionIndex = Index;
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

//처형 판별
AActor* UCDoAction::SearchCanExecut()
{
	FVector start = OwnerCharacter->GetActorLocation();
	FVector end = start + OwnerCharacter->GetActorForwardVector() * 100;
	TArray<AActor*> ignore;
	ignore.Add(Cast<AActor>(OwnerCharacter));
	FHitResult hitResult;

	bool result = UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery3, false,
		ignore, EDrawDebugTrace::Type::None, hitResult, true);

	if (result == false)
		return nullptr;

	if (!hitResult.GetActor()->GetComponentByClass<UStateComponent>()->IsGroggyMode())
		return nullptr;

	return hitResult.GetActor();
}

//처형
void UCDoAction::Execute(class ACEnemyCharacter* Target)
{
	ExecuteTarget = Target;
	MoveComponent->SetCamerafix(true);

	OwnerCharacter->PlayMontage_Server(ExecuteAnimData.ExecuteAnimation_Executioner);
	Target->PlayMontage_Server(ExecuteAnimData.ExecuteAnimation_Target);

	//처형 축 보정
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

	//캐릭터 런치
	LaunchCharacter->LaunchCharacter(launchVector, true, true);
	//공격자도 함께 런치
	if (DoActionData.bWithLaunch)
		OwnerCharacter->LaunchCharacter(launchVector, true, true);
}

void UCDoAction::ApplyDamage(AActor* OtherActor, class AAttachment* Attachment, const FHitResult& HitResult, bool isNormalHit)
{
	//UKismetSystemLibrary::DrawDebugSphere(this, HitResult.Location, 10, 12, FLinearColor::White, 5);
	SpawnHitEffect_Server(HitResult.Location);
	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();


	UStateComponent* otherStateComponent = OtherActor->GetComponentByClass<UStateComponent>();
	ACBaseCharacter* OtherCharacter = Cast<ACBaseCharacter>(OtherActor);
	if (otherStateComponent == nullptr)
		return;

	//카메라 쉐이크
	if (Cast<ACPlayerCharacter>(OwnerCharacter) == nullptr)					//몬스터가 한 공격
	{
		if (controller != nullptr && DoActionDatas[ActionIndex].CameraShakeClass != nullptr)
		{
			controller->PlayerCameraManager->StartCameraShake(DoActionDatas[ActionIndex].CameraShakeClass);
		}
	}
	else                                                                    //플레이어가 한 공격
	{
		if (controller != nullptr && DoActionDatas[ActionIndex - 1].CameraShakeClass != nullptr)
		{
			controller->PlayerCameraManager->StartCameraShake(DoActionDatas[ActionIndex - 1].CameraShakeClass);
		}
	}

	//피격 대상별 행동
	if (Cast<ACPlayerCharacter>(OtherCharacter) != nullptr)
	{
		OtherCharacter->StateComponent->SetHittedMode();
	}
	else
	{
		Cast<AAIController>(OtherCharacter->GetController())->GetBlackboardComponent()->SetValueAsObject("AttackTarget", Cast<UObject>(OwnerCharacter));
	}

	if (OtherCharacter->HasAuthority())
	{
		otherStateComponent->SubHP(currentDoActionData.Power);
	}

	if (OtherCharacter->HitNormalAnim == nullptr || OtherCharacter->NockDownAnim == nullptr)
		return;

	//공격 강함에 따른 애니메이션 출력
	if (isNormalHit)
		OtherCharacter->PlayMontage_Server(OtherCharacter->HitNormalAnim);
	else
		OtherCharacter->PlayMontage_Server(OtherCharacter->NockDownAnim);


}

void UCDoAction::SpawnHitEffect_Server_Implementation(FVector Location)
{
	SpawnHitEffect_NMC(Location);
}

void UCDoAction::SpawnHitEffect_NMC_Implementation(FVector Location)
{
	if (currentDoActionData.HittEffect == nullptr)
		return;
	//피격 이펙트 생성
	OwnerCharacter->SpawnNiagara_NMC(currentDoActionData.HittEffect, Location, FRotator::ZeroRotator, FVector(0.2f, 0.2f, 0.2f));
}
