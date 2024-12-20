// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Misc/Structures.h"
#include "EvadeComponent.generated.h"


struct FInputActionValue;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUST_API UEvadeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEvadeComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateMoveAxis(const FInputActionValue& Value);

	void PlayMontage(const FEvadeData& MontageData);

	UFUNCTION(Reliable, Server)
	void Evade_Server();
	UFUNCTION(Reliable, NetMulticast)
	void Evade_NMC(const FVector2D& value);
private:
	TWeakObjectPtr<class ACharacter> OwnerCharacter;
	FVector2D MovementVector;
private:
	UPROPERTY(EditAnywhere, Category = "Evade")
	FEvadeData EvadeDataF;
	UPROPERTY(EditAnywhere, Category = "Evade")
	FEvadeData EvadeDataB;
	UPROPERTY(EditAnywhere, Category = "Evade")
	FEvadeData EvadeDataL;
	UPROPERTY(EditAnywhere, Category = "Evade")
	FEvadeData EvadeDataR;

private:
	class UStateComponent* StateComponent;
};
