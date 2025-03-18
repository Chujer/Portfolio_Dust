#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataAsset/WeaponDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerSaveComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUST_API UPlayerSaveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerSaveComponent();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void LoadData();

	UFUNCTION()
	void LoadSetWeaponData();

	UFUNCTION(BlueprintCallable)
	void SaveData();

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCPlayerSaveGame> SaveGameClass;

	UPROPERTY()
	class UCPlayerSaveGame* SaveGame;
	
private:
	TWeakObjectPtr<class ACharacter> OwnerCharacter;
	FString FilePath;
};

