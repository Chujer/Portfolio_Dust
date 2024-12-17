#include "Character/CPlayerCharacter.h"

#include "CLog.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Component/MoveComponent.h"
#include "Component/PlayerSaveComponent.h"
#include "Component/StateComponent.h"
#include "Component/WeaponComponent.h"
#include "Components/TextRenderComponent.h"
#include "Controller/CLobbyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameInstance/CGameInstance.h"
#include "Interface/InteractionInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/CHelpers.h"


ACPlayerCharacter::ACPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;


	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 

	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.0f; 
	SpringArmComponent->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false; 

	MoveComponent = CreateDefaultSubobject<UMoveComponent>("MoveComponent");
	MoveComponent->SetIsReplicated(true);
	SaveComponent = CreateDefaultSubobject<UPlayerSaveComponent>("SaveComponent");
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
	WeaponComponent->SetIsReplicated(true);
	StateComponent = CreateDefaultSubobject<UStateComponent>("StateComponent");
	StateComponent->SetIsReplicated(true);

	///////////////////////////////////////////////////
	InteractText = CreateDefaultSubobject<UTextRenderComponent>("InteractText");
	InteractText->SetupAttachment(RootComponent);
	InteractText->SetRelativeLocation(FVector(0, 0, 120.0f));
	InteractText->SetRelativeRotation(FRotator(0, 180.0f, 0));
	InteractText->SetText(FText::FromString("G"));
	//////////////////////////////////////////////////
}

void ACPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	InteractText->SetVisibility(false);
	InteractionObject = nullptr;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//마지막 클라이언트의 캐릭터가 생성된 경우 
	if (Controller != nullptr && Controller->IsLocalController())
	{
		ACLobbyController* lobbyController = Cast<ACLobbyController>(Controller);
		UCGameInstance* gameInstance = Cast<UCGameInstance>(GetGameInstance());
		if (lobbyController == nullptr || gameInstance == nullptr)
			return;

		if (lobbyController->ConnectedPlayerInfo.Num() >= gameInstance->MissionPlayerCount)
		{
			LastPlayerInGame_Server();
		}
	}
}

void ACPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(UEnhancedInputComponent* Input = Cast< UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered,MoveComponent.Get(), &UMoveComponent::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, MoveComponent.Get(), &UMoveComponent::Look);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, MoveComponent.Get(), &UMoveComponent::Look);
		Input->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::PlayInteract);
		Input->BindAction(ActionAction, ETriggerEvent::Triggered, WeaponComponent.Get(), &UWeaponComponent::DoAction_Server);
	}
}


void ACPlayerCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	IInteractionInterface* other = Cast<IInteractionInterface>(OtherActor);
	if(!!other)
	{
		//거리에 비례해서 현재 활성화할 상호작용 객체 설정   -- TODO : 2024.07.04 -  임시 제작 차후 다른 곳으로 옮기거나 제거할 수 있음 
		FVector2D playerLocation = FVector2D(GetActorLocation().X, GetActorLocation().Y);
		FVector2D otherLocation = FVector2D(OtherActor->GetActorLocation().X, OtherActor->GetActorLocation().Y);
		float otherDistance = UKismetMathLibrary::Distance2D(playerLocation, otherLocation);
		//InteractionObject = other;
		if (!InteractionObject) //상호작용 객체가 없다면
		{
			InteractionObject = other;
			InteractionDistance = otherDistance;
		}
		else
		{
			if (InteractionDistance > otherDistance)
			{
				InteractionObject = other;
				InteractionDistance = otherDistance;
			}
		}
		InteractText->SetVisibility(true);

	}
}

void ACPlayerCharacter::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	IInteractionInterface* other = Cast<IInteractionInterface>(OtherActor);
	if (!!other && InteractionObject == other)
	{
		InteractionObject = nullptr;
		InteractionDistance = 0;
		InteractText->SetVisibility(false);
	}
}

void ACPlayerCharacter::PlayInteract()
{
	if(!!InteractionObject)
		InteractionObject->Interact(this);
}

void ACPlayerCharacter::LastPlayerInGame_Server_Implementation()
{
	AGameModeBase* gameMode = UGameplayStatics::GetGameMode(this);
	if (gameMode == nullptr)
		return;

	int playerCount = gameMode->GetNumPlayers();

	//게임모드 안의 모든 컨트롤러들의 LoadSetWeaponData(무기불러오기)를 실행
	for(int i = 0; i < playerCount; ++i)
	{
		ACPlayerCharacter* player = Cast<ACPlayerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), i)->GetPawn());
		if(player == nullptr)
			continue;
		player->LastPlayerInGame_Client();
	}
}

void ACPlayerCharacter::LastPlayerInGame_Client_Implementation()
{
	if (WeaponComponent == nullptr)
		return;
	WeaponComponent->LoadSetWeaponData();
}

