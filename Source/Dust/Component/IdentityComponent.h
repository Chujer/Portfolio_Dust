#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IdentityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUST_API UIdentityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UIdentityComponent();

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(Reliable, Server)
	void SetIdentity(TSubclassOf<class AIdentityObject> IdentityClass);

	UFUNCTION()
	void BeginIdentity();

	UFUNCTION()
	void EndIdentity();


public:
	UPROPERTY(Replicated)
	class AIdentityObject* Identity;

private:
	TWeakObjectPtr<ACharacter> OwnerCharacter;
};
