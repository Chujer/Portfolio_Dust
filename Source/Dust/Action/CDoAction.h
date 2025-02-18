#pragma once

#include "CoreMinimal.h"
#include "Misc/Structures.h"
#include "UObject/NoExportTypes.h"
#include "CDoAction.generated.h"

UCLASS(Blueprintable)
class DUST_API UCDoAction : public UObject
{
	GENERATED_BODY()

public:
	UCDoAction();
	
public:
	virtual void BeginPlay(class ACBaseCharacter* InOwner);

public:
	virtual void DoActionTrigger();
	virtual void DoAction_Server();
	virtual void DoAction_NMC();
	virtual void DoIndexAction_NMC(int Index);
	virtual void EndDoAtion_Server();
	virtual void EndDoAtion_NMC();

	virtual void DoActionRight_Server();
	virtual void DoActionRight_NMC();

	virtual AActor* SearchCanExecut();
	virtual void Execute(class ACEnemyCharacter* Target);

public:
	ACBaseCharacter* GetExecuteTarget() { return ExecuteTarget; }

public:
	void LaunchCharacter(FDoActionData DoActionData, class ACharacter* LaunchCharacter);

	UFUNCTION()
	void ApplyDamage(AActor* OtherActor, class AAttachment* Attachment, const FHitResult& HitResult);
	
	void SpawnHitEffect(FVector Location);

protected:
	TWeakObjectPtr<class ACBaseCharacter> OwnerCharacter;

protected:
	TWeakObjectPtr<class UStateComponent> StateComponent;
	TWeakObjectPtr<class UMoveComponent> MoveComponent;

protected:
	unsigned int ActionIndex = 0;
	unsigned int MaxActionIndex = 0;

public:
	UPROPERTY(EditAnywhere)
	TArray<struct FDoActionData> DoActionDatas;

private:
	UPROPERTY(EditAnywhere)
	struct FExecuteAnimData ExecuteAnimData;

	UPROPERTY(EditAnywhere)
	struct FDoActionData DoSubActionData;

	FDoActionData currentDoActionData;

private:
	ACBaseCharacter* ExecuteTarget;
};
