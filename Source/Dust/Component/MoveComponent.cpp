#include "Component/MoveComponent.h"

#include "GameFramework/Character.h"
#include "GameInstance/CGameInstance.h"
#include "Net/UnrealNetwork.h"

UMoveComponent::UMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UMoveComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMoveComponent, IsStop);
}


void UMoveComponent::Move(const FInputActionValue& Value)
{
	if (IsStop == true)
		return;

	// input is a Vector2D
	MovementVector = Value.Get<FVector2D>();

	if (OwnerCharacter->GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = OwnerCharacter->GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		OwnerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
		OwnerCharacter->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void UMoveComponent::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	APlayerController* playerController = Cast<APlayerController>(OwnerCharacter->GetController());

	if (IsCamerafix == true)
		return;

	if (!!playerController && playerController->bShowMouseCursor == false)
	{
		float mouseSenceX = Cast<UCGameInstance>(OwnerCharacter->GetGameInstance())->MouseSenceX;
		float mouseSenceY = Cast<UCGameInstance>(OwnerCharacter->GetGameInstance())->MouseSenceY;

		// add yaw and pitch input to controller
		OwnerCharacter->AddControllerYawInput(LookAxisVector.X * mouseSenceX);
		OwnerCharacter->AddControllerPitchInput(LookAxisVector.Y * mouseSenceY);
	}
}