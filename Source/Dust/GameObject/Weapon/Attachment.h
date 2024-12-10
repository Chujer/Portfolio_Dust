#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attachment.generated.h"

//TODO 2024.12.05 : 구조적 문제가 매우 심함 전체적으로 뜯어 고칠 필요가 있어보임

UCLASS()
class DUST_API AAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttachment();
	
protected:
	virtual void BeginPlay() override;
	
private:
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	UPROPERTY(EditAnywhere, Category = "Attachment")
	FName AttachSocketName;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WeaponMesh1;
	

private:
};
