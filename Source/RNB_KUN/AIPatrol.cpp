// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "AIPatrol.h"
#include "AIPatrolController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "Engine.h"


// Sets default values
AAIPatrol::AAIPatrol()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SetPeripheralVisionAngle(90.f);
}

// Called when the game starts or when spawned
void AAIPatrol::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAIPatrol::OnPlayerCaught);
	}
}

// Called to bind functionality to input
void AAIPatrol::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AAIPatrol::OnPlayerCaught(APawn* Pawn)
{
	/** passes the caught pawn to AI Controller, which store its in Blackboard further  */
	AAIPatrolController* AIController = Cast<AAIPatrolController>(GetController());
	if (AIController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("You have been caught...")));
		AIController->SetPlayerCought(Pawn);
	}
}

