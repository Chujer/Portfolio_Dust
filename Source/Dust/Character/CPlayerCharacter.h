#pragma once

#include "CoreMinimal.h"
#include "DustCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "CPlayerCharacter.generated.h"

UCLASS()
class DUST_API ACPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACPlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	//TODO - 임시 실행 함수 /////////////////////////////////////////
public:
	void PlayInteract();

	UPROPERTY(EditAnywhere)
		class UTextRenderComponent* InteractText;

		void LoadPlayerData();

		UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
		void LoadPlayerData_NMC();

	//TODO/////////////////// /////////////////////////////////////////
		
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	 UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	 UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	 UInputAction* LookAction;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	 UInputAction* InteractionAction;
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
		TObjectPtr<class UMoveComponent> MoveComponent;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TObjectPtr<class UPlayerSaveComponent> SaveComponent;	

		TObjectPtr<class IInteractionInterface> InteractionObject;
	UPROPERTY()
		float InteractionDistance = 10000.0f;
};
