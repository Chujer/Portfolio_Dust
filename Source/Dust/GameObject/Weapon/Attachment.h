#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attachment.generated.h"

//TODO 2024.12.05 : ������ ������ �ſ� ���� ��ü������ ��� ��ĥ �ʿ䰡 �־��

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
	//�ش� _Server �Լ����� WeaponComponent���� Server�� ȣ���ϱ� ������ UFUNCTION(Server)�� ȣ������ ����
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
