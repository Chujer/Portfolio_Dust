#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FeetComponent.generated.h"

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Feet")
	FVector LeftDistacne;   //X

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Feet")
	FVector RightDistacne;  //Y

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Feet")
	FVector PelvisDistance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Feet")
	FRotator LeftRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Feet")
	FRotator RightRotation;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUST_API UFeetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFeetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void Trace(FName InName, float& OutDistance, FRotator& OutRotation);

public:
	FORCEINLINE FFeetData GetData() { return Data; }

private:
	UPROPERTY(EditAnywhere, Category = "Trace")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug; //������

	UPROPERTY(EditAnywhere, Category = "Trace")
	float InterpSpeed = 50; //���� �ӵ�

	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceDistance = 50; //�������� Trace ��ȿ ����

	UPROPERTY(EditAnywhere, Category = "Trace")
	float OffsetDistance = 5; // ������ ���ִ� ��

	UPROPERTY(EditAnywhere, Category = "Socket")
	FName LeftSocek = "Foot_L";

	UPROPERTY(EditAnywhere, Category = "Socket")
	FName RightSocek = "Foot_R";

private:
	FFeetData Data;
	class ACBaseCharacter* OwnerCharacter;

};
