// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "Kismet/GameplayStatics.h"
#include "CameraUtility.h"


// Sets default values
ACameraUtility::ACameraUtility()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraUtility::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACameraUtility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float TimeBetweenCameraChanges = 2;
	const float SmoothBlendTime = 0.75;

	TimeToNextCameraChange -= DeltaTime;

	if (TimeToNextCameraChange <= 0)
	{
		// restore initial value
		TimeToNextCameraChange += TimeBetweenCameraChanges;

		APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);

		if (OurPlayerController)
		{
			if (CameraTwo && (OurPlayerController->GetViewTarget() == CameraOne))
			{
				OurPlayerController->SetViewTargetWithBlend(CameraTwo, SmoothBlendTime);
			}
			else if (CameraOne)
			{
				OurPlayerController->SetViewTargetWithBlend(CameraOne);
			}
		}
	}

}

