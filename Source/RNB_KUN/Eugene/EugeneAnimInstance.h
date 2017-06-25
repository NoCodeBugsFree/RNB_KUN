// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EugeneAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RNB_KUN_API UEugeneAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	// NativeUpdateAnimation
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
		
protected:

	/** Character's current movement speed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;
		
};
