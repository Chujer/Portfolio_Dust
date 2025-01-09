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
	virtual void DoActionTrigger();
	virtual void DoAction_Server();
	virtual void DoAction_NMC();
	virtual void DoIndexAction_NMC(int Index);
	virtual void EndDoAtion_Server();
	virtual void EndDoAtion_NMC();

public:
	void LaunchCharacter(FDoActionData DoActionData, class ACharacter* LaunchCharacter);

	UFUNCTION()
	void ApplyDamage(AActor* OtherActor, class AAttachment* Attachment, const FHitResult& HitResult);
	
	void SpawnHitEffect(FVector Location);

protected:
	TWeakObjectPtr<ACharacter> OwnerCharacter;

protected:
	TWeakObjectPtr<class UStateComponent> StateComponent;
	TWeakObjectPtr<class UMoveComponent> MoveComponent;

protected:
	unsigned int ActionIndex = 0;
	unsigned int MaxActionIndex = 0;

private:
	UPROPERTY(EditAnywhere)
	TArray<struct FDoActionData> DoActionDatas;

	FDoActionData currentDoActionData;
};
