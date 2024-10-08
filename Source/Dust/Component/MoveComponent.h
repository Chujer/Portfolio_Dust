#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUST_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMoveComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	FVector2D GetMovementVector() { return MovementVector; }

private:
	TWeakObjectPtr<class ACharacter> OwnerCharacter;

	//키입력값이 저장될 변수
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FVector2D MovementVector;
};
