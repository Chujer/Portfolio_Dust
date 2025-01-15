#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IdentityObject.generated.h"

UCLASS()
class DUST_API AIdentityObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AIdentityObject();

protected:
	virtual void BeginPlay() override;

public:
	virtual void BeginIdentity();
	virtual void EndIdentity();

public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* IdentityMesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UShapeComponent* Collision;

protected: 
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	UPROPERTY(EditAnywhere, Category = "Attachment")
	FName AttachSocketName;
};
