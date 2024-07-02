// Copyright Epic Games, Inc. All Rights Reserved.

#include "DustGameMode.h"
#include "DustCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADustGameMode::ADustGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
