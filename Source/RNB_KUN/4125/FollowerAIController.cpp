// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "FollowerAIController.h"
#include "FollowerCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AFollowerAIController::AFollowerAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void AFollowerAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	
	// Initialize Blackboard and Start BT
	AFollowerCharacter* AICharacter = Cast<AFollowerCharacter>(InPawn);
	if (AICharacter)
	{
		if (AICharacter->BehaviorTree->BlackboardAsset)
		{
			BlackboardComponent->InitializeBlackboard(*(AICharacter->BehaviorTree->BlackboardAsset));
		}

		BehaviorTreeComponent->StartTree(*AICharacter->BehaviorTree);
	}

}

void AFollowerAIController::SetPlayerToFollow(APawn* Pawn)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsObject("PlayerToFollow", Pawn);
	}
}
