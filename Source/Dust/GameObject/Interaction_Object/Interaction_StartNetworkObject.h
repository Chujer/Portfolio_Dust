#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionInterface.h"
#include "Interaction_StartNetworkObject.generated.h"

UCLASS()
class DUST_API AInteraction_StartNetworkObject : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AInteraction_StartNetworkObject();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(ACharacter* Player) override;

	//Main Menu UI생성
	//블루프린트에서 설정하기 위해서 nativeEvent사용
	UFUNCTION(BlueprintNativeEvent)
	void OpenNetworkUI(ACharacter* player);
	void OpenNetworkUI_Implementation(ACharacter* Player);
};
