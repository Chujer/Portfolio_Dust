#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataAsset/WeaponDataAsset.h"
#include "PlayerSaveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUST_API UPlayerSaveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerSaveComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void LoadData();

	UFUNCTION(BlueprintCallable)
	void SaveData(UWeaponDataAsset* data);

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCPlayerSaveGame> SaveGameClass;

	UPROPERTY()
	class UCPlayerSaveGame* SaveGame;

private:
	TWeakObjectPtr<class ACharacter> OwnerCharacter;
	FString FilePath;
};

