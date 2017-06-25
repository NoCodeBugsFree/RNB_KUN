// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "BTTaskNode_SelectPatrolPoint.h"
#include "AIPatrolPoint.h"
#include "AIPatrolController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTaskNode_SelectPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/** if our AI Controller is valid  */
	AAIPatrolController* AIPatrolController = Cast<AAIPatrolController>(OwnerComp.GetAIOwner());
	if (AIPatrolController)
	{
		UBlackboardComponent* BlackboardComponent = AIPatrolController->GetBlackboardComponent();
		
		/** and have a valid BB  */
		if (BlackboardComponent)
		{
			/** Current AIPatrolPoint */
			AAIPatrolPoint* AIPatrolPoint = Cast<AAIPatrolPoint>(BlackboardComponent->GetValueAsObject("LocationToGo"));

			/** All AIPatrolPoints  */
			TArray<AActor*> AvailablePatrolPoints = AIPatrolController->GetPatrolPoints();

			/** Where we intend to go  */
			AAIPatrolPoint* NextPatrolPoint = nullptr;

			/** if we have at least one AIPatrolPoint in  AvailablePatrolPoints array */
			if (AvailablePatrolPoints.Num() > 0)
			{
				/** if Current PP is not last PP in array  */
				if (AIPatrolController->CurrentPatrolPoint != AvailablePatrolPoints.Num() - 1)
				{
					/** simple increment PP number  */
					NextPatrolPoint = Cast<AAIPatrolPoint>(AvailablePatrolPoints[++AIPatrolController->CurrentPatrolPoint]);
				}
				else /** if Current PP is last PP in array set it to zero  */
				{
					NextPatrolPoint = Cast<AAIPatrolPoint>(AvailablePatrolPoints[0]);
					AIPatrolController->CurrentPatrolPoint = 0;
				}

				/** Refresh the BlackboardComponent with NextPatrolPoint  */
				BlackboardComponent->SetValueAsObject("LocationToGo", NextPatrolPoint);
			}
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
