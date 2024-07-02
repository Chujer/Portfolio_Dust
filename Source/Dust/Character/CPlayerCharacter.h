#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Component/MoveComponent.h"
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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TWeakObjectPtr<UMoveComponent> MoveComponent;
};
