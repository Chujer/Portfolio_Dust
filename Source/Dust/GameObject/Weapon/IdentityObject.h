#pragma once

#include "CoreMinimal.h"
#include "Components/ShapeComponent.h"
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
	virtual void SetCollision(ECollisionEnabled::Type value) { Collision->SetCollisionEnabled(value); }

	virtual void BeginIdentity();
	virtual void EndIdentity();

	virtual void BeginIdentitySkill();
	virtual void EndIdentitySkill();

public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* IdentityMesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UShapeComponent* Collision;

protected: 
	TWeakObjectPtr<class ACBaseCharacter> OwnerCharacter;
	UPROPERTY(EditAnywhere, Category = "Attachment")
	FName AttachSocketName;
};
