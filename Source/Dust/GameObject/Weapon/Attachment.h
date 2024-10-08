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

protected:
	virtual void BeginPlay() override;

public:
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	UPROPERTY(EditAnywhere, Category = "Attachment")
		FName AttachSocketName;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* WeaponMesh1;
};
