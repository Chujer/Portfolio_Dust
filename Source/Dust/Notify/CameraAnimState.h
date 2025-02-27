// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CameraAnimState.generated.h"

UCLASS()
class DUST_API UCameraAnimState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	FString GetNotifyName_Implementation() const override;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACameraAnimActor> CameraAnimActorClass;

	class ACameraAnimActor* CameraAnimActor;

	UPROPERTY(EditAnywhere)
	class UCameraAnimationSequence* AnimSequence;
	
	class ACharacter* OwnerCharacter;

};