// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FollowerAIController.generated.h"

/**
 * 
 */
UCLASS()
class RNB_KUN_API AFollowerAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AFollowerAIController();

	virtual void Possess(APawn* InPawn) override;

	/** Sets target to follow to BlackBoard  */
	void SetPlayerToFollow(APawn* Pawn);

protected:
	
	/** Blackboard Component  */
	UPROPERTY()
	class UBlackboardComponent* BlackboardComponent;

	/** BehaviorTree Component  */
	UPROPERTY()
	class UBehaviorTreeComponent* BehaviorTreeComponent;
	
};
