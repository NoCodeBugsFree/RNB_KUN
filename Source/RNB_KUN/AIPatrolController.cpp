// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "AIPatrolController.h"
#include "AIPatrol.h"
#include "AIPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "RNB_KUNCharacter.h"

AAIPatrolController::AAIPatrolController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void AAIPatrolController::BeginPlay()
{
	Super::BeginPlay();
}

void AAIPatrolController::SetPlayerCought(APawn * Pawn)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsObject(PlayerKey, Pawn);
	}
}

void AAIPatrolController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);
	
	// character reference
	AAIPatrol* AICharacter = Cast<AAIPatrol>(Pawn);
	if (AICharacter)
	{
		if (AICharacter->BehaviorTree->BlackboardAsset)
		{
			BlackboardComponent->InitializeBlackboard(*(AICharacter->BehaviorTree->BlackboardAsset));
		}

		// populate patrol points array
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPatrolPoint::StaticClass(), PatrolPoints);

		BehaviorTreeComponent->StartTree(*AICharacter->BehaviorTree);
	}
}
