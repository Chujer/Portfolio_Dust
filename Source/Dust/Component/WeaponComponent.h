// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndBeginWeaponComp);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUST_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponComponent();

public:
	

protected:
	virtual void BeginPlay() override;

public:
	TWeakObjectPtr<class AAttachment> GetAttachment() const; ;
	TWeakObjectPtr<class UCDoAction> GetDoAction() const; ;
	class UWeaponDataAsset* GetWeaponDataAsset() { return WeaponDataAsset; }
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(Reliable, Server)
	void DoAction_Server();

	UFUNCTION(Reliable, NetMulticast)
	void DoAction_NMC();

	UFUNCTION(Reliable, Server)
	void EndDoAction_Server();



public:
	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SetWeaponData_Server(int WeaponIndex);

	//Attachment의 경우 리플리케이션한 엑터이므로 밖(Server)에서 생성후 매개변수로 가져옴
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast)
	void SetWeaponData_NMC(int WeaponIndex, AAttachment* Attachment);

	UFUNCTION()
	void SetWeaponAnimInstance_NMC();

public:
	////저장한 파일을 읽어 무기 설정
	UFUNCTION(BlueprintCallable)
	void LoadSetWeaponData();
private:
	TWeakObjectPtr<class UStateComponent> StateComponent;
	TWeakObjectPtr<class UPlayerSaveComponent> SaveComponent;

private:
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	
	class UWeaponDataAsset* WeaponDataAsset;
	class UWeaponData* WeaponData;

public:
	UPROPERTY(BlueprintReadOnly)
	int curWeaponIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* DataTable;
	
};
