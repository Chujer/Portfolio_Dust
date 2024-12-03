// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUST_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(Reliable, Server)
		void DoAction();
public:
	//무기 설정
	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SetWeaponData(class UWeaponDataAsset* weaponData);

	UFUNCTION(NetMulticast, Reliable)
	void SetWeaponAnimInstance(UWeaponDataAsset* weaponData);

private:
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	UPROPERTY(EditAnywhere, Replicated)
	TWeakObjectPtr<class AAttachment> CurAttachment;
	UWeaponDataAsset* CurWeaponData;
};
