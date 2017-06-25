// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "EugeneAnimInstance.h"
#include "RNB_KUNCharacter.h"

void UEugeneAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	APawn* TestPawn = TryGetPawnOwner();
	if (TestPawn)
	{
		MovementSpeed = TestPawn->GetVelocity().Size();
	}
}
