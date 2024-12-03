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
	virtual void BeginPlay(class ACharacter* InOwner);

public:
	virtual void DoAction();
	virtual void NextDoAction();
	virtual void EndDoAtion();

public:
	//TODO : 2024.10.11 : DoAction�ִϸ��̼��� ����Ŭ���̾�Ʈ������ ���̴� ���� �߻� �ذ� ���
	void PlayMontage();
	void LaunchCharacter(FDoActionData DoActionData, class ACharacter* LaunchCharacter);

private:
	TWeakObjectPtr<class UStateComponent> StateComponent;
	TWeakObjectPtr<class UMoveComponent> MoveComponent;

private:
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> DoActionDatas;

private:
	unsigned int Index = 0;
	unsigned int MaxIndex = 0;

};
