// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInctance_Base.generated.h"

/**
 * 
 */
UCLASS()
class DUST_API UCAnimInctance_Base : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	//캐릭터 정보를 받는 변수
	UPROPERTY(BlueprintReadOnly, Category = "Character_Info")
		float Speed;
	UPROPERTY(BlueprintReadOnly, Category = "Character_Info")
		float Pitch;
	UPROPERTY(BlueprintReadOnly, Category = "Character_Info")
		float Direction;
private:
	TWeakObjectPtr<ACharacter> Character;
	FRotator InterpRotation;
};
