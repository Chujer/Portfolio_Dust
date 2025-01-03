// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAIController.h"

#include "CLog.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

ABaseAIController::ABaseAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
	SetPerceptionComponent(*AIPerception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfig");
	SightConfig->SightRadius = 800.0f;
	SightConfig->LoseSightRadius = 1200.f;
	SightConfig->PeripheralVisionAngleDegrees = 60.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	AIPerception->OnPerceptionUpdated.AddDynamic(this, &ABaseAIController::PerceptionUpdated);
	
}

void ABaseAIController::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{

	if (Blackboard == nullptr)
		return;

	if (Blackboard.Get()->GetValueAsObject("AttackTarget") == nullptr)
		Blackboard->SetValueAsObject("AttackTarget", UpdatedActors[0]);
}
