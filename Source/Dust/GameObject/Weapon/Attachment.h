#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Attachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBeginCollision, AActor*, OtherActor, class AAttachment*, Attachment);

UCLASS()
class DUST_API AAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttachment();

public:
	void SetCollision(ECollisionEnabled::Type value) { Collision->SetCollisionEnabled(value); }

protected:
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	UPROPERTY(EditAnywhere, Category = "Attachment")
	FName AttachSocketName;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WeaponMesh1;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* Collision;
	

public:
	FOnBeginCollision OnBeginCollision;
};
