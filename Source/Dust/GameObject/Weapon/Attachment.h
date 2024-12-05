#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attachment.generated.h"

//TODO 2024.12.05 : 구조적 문제가 매우 심함 전체적으로 뜯어 고칠 필요가 있어보임

UCLASS()
class DUST_API AAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttachment();

	FORCEINLINE bool IsComboReady() { return ComboReady; }
	FORCEINLINE void SetIsNext(const bool& isNext) { IsNext = isNext; }
	FORCEINLINE void SetIsComboReady(const bool& isCombo) { ComboReady = isCombo;	}

protected:
	virtual void BeginPlay() override;

public:
	//해당 _Server 함수들은 WeaponComponent에서 Server로 호출하기 때문에 UFUNCTION(Server)를 호출하지 않음
	void StartAction_Server();
	void EndDoAction_Server();
	virtual void DoActionCombo_Server();

	UFUNCTION(NetMulticast,Reliable)
	void DoAction_NMC(const struct FDoActionData& DoActionData);

private:
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	UPROPERTY(EditAnywhere, Category = "Attachment")
	FName AttachSocketName;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WeaponMesh1;

	UPROPERTY(EditAnywhere)
	TWeakObjectPtr<class UCDoAction> DoAction;											
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
	TArray<struct FDoActionData> DoActionDatas;

private:
	unsigned int ActionIndex = 0;
	unsigned int MaxActionIndex = 0;

	bool IsNext = false;
	bool ComboReady = false;
};
