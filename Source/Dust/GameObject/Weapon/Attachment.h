#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Attachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnBeginCollision, AActor*, OtherActor, class AAttachment*, Attachment, const FHitResult&, HitResult, bool, isNormalHit);

UCLASS()
class DUST_API AAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttachment();

public:
	UFUNCTION(Reliable, Server)
	void AddIgnore(AActor* Actor);

	UFUNCTION(Reliable, Server)
	void SetCollision(ECollisionEnabled::Type value); 

	UFUNCTION(Reliable, Server)
	void ClearHittedCharacter();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UCapsuleComponent* Collision;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WeaponMesh;

	bool isNormalDamage = true;

private:
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	UPROPERTY(EditAnywhere, Category = "Attachment")
	FName AttachSocketName;

	TArray<class ACBaseCharacter*> HittedCharacter;
	TArray<class ACBaseCharacter*> invincibilityCharacter;

public:
	FOnBeginCollision OnBeginCollision;
};
