#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attachment.generated.h"

UCLASS()
class DUST_API AAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttachment();
	

public:
	UFUNCTION(Server,Reliable)
	void DoAction_Server();

	UFUNCTION(NetMulticast,Reliable)
	void DoAction_NMC();
	
protected:
	virtual void BeginPlay() override;

public:
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	UPROPERTY(EditAnywhere, Category = "Attachment")
		FName AttachSocketName;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* WeaponMesh1;
	UPROPERTY(EditAnywhere)
		TWeakObjectPtr<class UCDoAction> DoAction;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCDoAction> DoActionClass;
};
