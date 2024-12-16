#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataAsset/WeaponDataAsset.h"
#include "PlayerSaveComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPostSaveCompBeginPlay);

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

	UFUNCTION()
	void LoadSetWeaponData();

	UFUNCTION(BlueprintCallable)
	void SaveData();

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCPlayerSaveGame> SaveGameClass;

	UPROPERTY()
	class UCPlayerSaveGame* SaveGame;

	// 설정한 컴포넌트의 BeginPlay가 완료된 순간
	FOnPostSaveCompBeginPlay OnPostComponentBeginPlay;

private:
	TWeakObjectPtr<class ACharacter> OwnerCharacter;
	FString FilePath;
};

