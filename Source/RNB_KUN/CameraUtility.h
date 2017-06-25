// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CameraUtility.generated.h"

UCLASS()
class RNB_KUN_API ACameraUtility : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACameraUtility();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

private:

	/** Camera One reference */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	AActor* CameraOne;

	/** Camera Two reference*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	AActor* CameraTwo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float TimeToNextCameraChange = 2;

};
