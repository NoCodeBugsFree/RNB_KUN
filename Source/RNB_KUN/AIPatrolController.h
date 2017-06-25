// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "AIPatrolController.generated.h"

/**
 * 
 */
UCLASS()
class RNB_KUN_API AAIPatrolController : public AAIController
{
	GENERATED_BODY()
	
public:

	AAIPatrolController();

	virtual void BeginPlay() override;

	/** Returns BlackboardComponent reference   */
	FORCEINLINE UBlackboardComponent * GetBlackboardComponent() const { return BlackboardComponent; }
	
	/** Returns the array of PatrolPoints  */
	FORCEINLINE TArray<AActor*> GetPatrolPoints() const { return PatrolPoints; }

	/** Stores the pawn reference to Blackboard  */
	void SetPlayerCought(APawn* Pawn);

	/** Current patrol point which character intended to check  */
	int32 CurrentPatrolPoint = 0;

private:
	
	/** Blackboard Component  */
	UPROPERTY()
	class UBlackboardComponent* BlackboardComponent;

	/** BehaviorTree Component  */
	UPROPERTY()
	class UBehaviorTreeComponent* BehaviorTreeComponent;
	
	/** Next patrol point -
	* in Blackboard 
	* name: LocationToGo
	* KeyType: (Object(Actor))  
	*/
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName LocationToGoKey = "LocationToGo"; // keys setup accordingly to Blackboard names

	/** 
	* Player Character
	* in Blackboard
	* name: Target
	* KeyType: (Object(Actor)) 
	*/
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName PlayerKey = "Target"; // keys setup accordingly to Blackboard names

	/** All patrol points to check by this AI  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> PatrolPoints;

	/**
	* Handles attaching this controller to the specified pawn.
	* Only runs on the network authority (where HasAuthority() returns true).
	* @param InPawn The Pawn to be possessed.
	* @see HasAuthority()
	*/
	virtual void Possess(APawn* Pawn) override;

};
