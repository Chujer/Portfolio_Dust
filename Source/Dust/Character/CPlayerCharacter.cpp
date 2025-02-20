#include "Character/CPlayerCharacter.h"

#include "CLog.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Component/EvadeComponent.h"
#include "Component/IdentityComponent.h"
#include "Component/MoveComponent.h"
#include "Component/PlayerSaveComponent.h"
#include "Component/StateComponent.h"
#include "Component/WeaponComponent.h"
#include "Components/TextRenderComponent.h"
#include "Controller/CLobbyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameInstance/CGameInstance.h"
#include "GameMode/CLobbyGameMode.h"
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
	EvadeComponent = CreateDefaultSubobject<UEvadeComponent>("EvadeComponent");
	EvadeComponent->SetIsReplicated(true);
	IdentityComponent = CreateDefaultSubobject<UIdentityComponent>("IdentityComponent");
	IdentityComponent->SetIsReplicated(true);

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

	//������ Ŭ���̾�Ʈ�� ĳ���Ͱ� ������ ��� 
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

	if (GetController() != nullptr && GetController()->IsLocalController())
	{
		StateComponent->MakeHPUI();
	}
}

void ACPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UseControllerRotationYaw�� Ȱ��ȭ�� ĳ������ ȸ���� �Ҳ��ܺ��̴� ���� �����ؼ� �ε巯�� ȸ������ ����
	if(EvadeToCameraFix)
	{
		FRotator currentRotation = GetActorRotation();
		FRotator targetRotation = GetActorRotation();
		targetRotation.Yaw = GetControlRotation().Yaw;
		FRotator newRotation = UKismetMathLibrary::RInterpTo(currentRotation, targetRotation, DeltaTime, 15.0f);
		SetActorRotation(newRotation);

		if (UKismetMathLibrary::NearlyEqual_FloatFloat(UKismetMathLibrary::NormalizeAxis(currentRotation.Yaw),
			UKismetMathLibrary::NormalizeAxis(targetRotation.Yaw), NealyControllerGap))
		{
			bUseControllerRotationYaw = true;
			IsUseControllerRotYaw = false;
			EvadeToCameraFix = false;
		}
	}
}

void ACPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(UEnhancedInputComponent* Input = Cast< UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered,MoveComponent.Get(), &UMoveComponent::Move);
		Input->BindAction(MoveAction, ETriggerEvent::Triggered,EvadeComponent.Get(), &UEvadeComponent::UpdateMoveAxis);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, MoveComponent.Get(), &UMoveComponent::Look);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, MoveComponent.Get(), &UMoveComponent::Look);
		Input->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::PlayInteract);
		Input->BindAction(ActionAction, ETriggerEvent::Triggered, WeaponComponent.Get(), &UWeaponComponent::DoAction_Server);
		Input->BindAction(EvadeAction, ETriggerEvent::Triggered, EvadeComponent.Get(), &UEvadeComponent::Evade_Server);
		Input->BindAction(IdentityStartAction, ETriggerEvent::Triggered, IdentityComponent.Get(), &UIdentityComponent::BeginIdentity);
		Input->BindAction(IdentityEndAction, ETriggerEvent::Triggered, IdentityComponent.Get(), &UIdentityComponent::EndIdentity);
		Input->BindAction(RightClickAction, ETriggerEvent::Triggered, IdentityComponent.Get(), &UIdentityComponent::BeginIdentitySkill);
		Input->BindAction(RightClickAction, ETriggerEvent::Triggered, WeaponComponent.Get(), &UWeaponComponent::DoActionRight_Server);
	}
}


void ACPlayerCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	IInteractionInterface* other = Cast<IInteractionInterface>(OtherActor);
	if(!!other)
	{
		//�Ÿ��� ����ؼ� ���� Ȱ��ȭ�� ��ȣ�ۿ� ��ü ����   -- TODO : 2024.07.04 -  �ӽ� ���� ���� �ٸ� ������ �ű�ų� ������ �� ���� 
		FVector2D playerLocation = FVector2D(GetActorLocation().X, GetActorLocation().Y);
		FVector2D otherLocation = FVector2D(OtherActor->GetActorLocation().X, OtherActor->GetActorLocation().Y);
		float otherDistance = UKismetMathLibrary::Distance2D(playerLocation, otherLocation);
		//InteractionObject = other;
		if (!InteractionObject) //��ȣ�ۿ� ��ü�� ���ٸ�
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
	ACLobbyGameMode* gameMode = Cast<ACLobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (gameMode == nullptr)
		return;

	int playerCount = gameMode->GetNumPlayers();


	//���Ӹ�� ���� ��� ��Ʈ�ѷ����� LoadSetWeaponData(����ҷ�����)�� ����
	for(int i = 0; i < playerCount; ++i)
	{
		ACPlayerCharacter* player = Cast<ACPlayerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), i)->GetPawn());
		if(player == nullptr)
			continue;
		player->LastPlayerInGame_NMC();
	}
	gameMode->OnLastPlayerInGame.Broadcast();
}

void ACPlayerCharacter::LastPlayerInGame_NMC_Implementation()
{
	if (WeaponComponent == nullptr)
		return;
	WeaponComponent->LoadSetWeaponData();
}

