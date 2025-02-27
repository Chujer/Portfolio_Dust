// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraAnimActor.generated.h"

UCLASS()
class DUST_API ACameraAnimActor : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartCameraAnim(class UCameraAnimationSequence* AnimSequence);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndCameraAnim();


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ACharacter* OwnerCharacter;
};
