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
	//TODO : 2024.10.11 : DoAction애니메이션이 서버클라이언트에서만 보이는 문제 발생 해결 요망
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
