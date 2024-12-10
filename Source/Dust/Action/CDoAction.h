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

public:
	virtual void BeginPlay(class ACharacter* InOwner);

public:
	virtual void DoAction_Server();
	virtual void EndDoAtion_Server();
	void DoAction_NMC();

public:
	void LaunchCharacter(FDoActionData DoActionData, class ACharacter* LaunchCharacter);

private:
	TWeakObjectPtr<class UStateComponent> StateComponent;
	TWeakObjectPtr<class UMoveComponent> MoveComponent;

private:
	TWeakObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(EditAnywhere)
	TArray<struct FDoActionData> DoActionDatas;
private:
	unsigned int ActionIndex = 0;
	unsigned int MaxActionIndex = 0;
};
