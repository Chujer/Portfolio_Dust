// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataAsset/WeaponData.h"
#include "DataAsset/WeaponDataAsset.h"
#include "WeaponComponent.generated.h"


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
	void SetWeaponData_Server(class UWeaponDataAsset* weaponDataAsset);

	//Attachment의 경우 리플리케이션한 엑터이므로 밖(Server)에서 생성후 매개변수로 가져옴
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast)
	void SetWeaponData_NMC(class UWeaponDataAsset* weaponDataAsset, AAttachment* Attachment);

	UFUNCTION()
	void SetWeaponAnimInstance_NMC();
	
private:
	TWeakObjectPtr<class UStateComponent> StateComponent;

private:
	TWeakObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(EditAnywhere)
	UWeaponDataAsset* WeaponDataAsset;
	
	UPROPERTY(EditAnywhere)
	class UWeaponData* WeaponData;
	
};
