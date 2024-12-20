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

	//TODO - �ӽ� ���� �Լ� /////////////////////////////////////////
public:
	void PlayInteract();

	UPROPERTY(EditAnywhere)
	class UTextRenderComponent* InteractText;
	
	UFUNCTION(BlueprintCallable)
	FString GetClientName() { return GetWorld()->GetFirstPlayerController()->GetName(); }

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void LastPlayerInGame_Server();
	UFUNCTION(Client, Reliable)
	void LastPlayerInGame_Client();


	
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

public:
	/////////////////////������Ʈ
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMoveComponent> MoveComponent;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UPlayerSaveComponent> SaveComponent;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UWeaponComponent> WeaponComponent;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UStateComponent> StateComponent;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UEvadeComponent> EvadeComponent;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TObjectPtr<class IInteractionInterface> InteractionObject;
	UPROPERTY()
		float InteractionDistance = 10000.0f;

private:
	//BeginPlay�� ���� Component�� ����
	int CompEndBeginCount = 0;

public:
	//UseControllerRotationYaw�� ���� ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UseControllerRotSetting")
	bool IsUseControllerRotYaw = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UseControllerRotSetting")
	float NealyControllerGap = 370.0f;
};
