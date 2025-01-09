#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Attachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBeginCollision, AActor*, OtherActor, class AAttachment*, Attachment, const FHitResult&, HitResult);

UCLASS()
class DUST_API AAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttachment();

public:
	void SetCollision(ECollisionEnabled::Type value) { Collision->SetCollisionEnabled(value); }

	void ClearHittedCharacter() { HittedCharacter.Empty(); }

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UCapsuleComponent* Collision;

private:
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	UPROPERTY(EditAnywhere, Category = "Attachment")
	FName AttachSocketName;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WeaponMesh1;

	TArray<ACharacter*> HittedCharacter;

public:
	FOnBeginCollision OnBeginCollision;
};
