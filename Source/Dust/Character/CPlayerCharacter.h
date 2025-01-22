#pragma once

#include "CoreMinimal.h"
#include "CBaseCharacter.h"
#include "DustCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "CPlayerCharacter.generated.h"



UCLASS()
class DUST_API ACPlayerCharacter : public ACBaseCharacter
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
	
	UFUNCTION(BlueprintCallable)
	FString GetClientName() { return GetWorld()->GetFirstPlayerController()->GetName(); }

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void LastPlayerInGame_Server();
	UFUNCTION(NetMulticast, Reliable)
	void LastPlayerInGame_NMC();


	
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

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	 UInputAction* ActionAction;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	 UInputAction* EvadeAction;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	 UInputAction* IdentityStartAction;
	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	 UInputAction* IdentityEndAction;
	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	 UInputAction* RightClickAction;

public:
	/////////////////////컴포넌트
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMoveComponent> MoveComponent;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UPlayerSaveComponent> SaveComponent;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UEvadeComponent> EvadeComponent;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UIdentityComponent> IdentityComponent;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TObjectPtr<class IInteractionInterface> InteractionObject;
	UPROPERTY()
		float InteractionDistance = 10000.0f;

private:
	//BeginPlay가 끝난 Component의 갯수
	int CompEndBeginCount = 0;

public:
	//UseControllerRotationYaw의 보간 설정
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UseControllerRotSetting")
	bool IsUseControllerRotYaw = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UseControllerRotSetting")
	float NealyControllerGap = 370.0f;
};
