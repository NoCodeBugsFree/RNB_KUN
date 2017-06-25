// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "Rotatable.h"
#include "RNB_KUNCharacter.h"

// Sets default values
ARotatable::ARotatable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ARotatable::OnAnimTriggered);

	RotatableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RotatableMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARotatable::BeginPlay()
{
	Super::BeginPlay();
	
	if (AnimCurve)
	{
		FOnTimelineFloat ProgressFunction;

		ProgressFunction.BindUFunction(this, FName("HandleProgress"));

		Timeline.AddInterpFloat(AnimCurve, ProgressFunction);
		Timeline.SetLooping(false);

		ActorInitialRotation = TargetRotation = GetActorRotation();

		if (RotationAxis == ERotationAxis::RA_Yaw)
		{
			TargetRotation.Yaw += MaxRotation;
		} 
		else if(RotationAxis == ERotationAxis::RA_Pitch)
		{
			TargetRotation.Pitch += MaxRotation;
		}
		else if (RotationAxis == ERotationAxis::RA_Roll)
		{
			TargetRotation.Roll += MaxRotation;
		}
	}

}

// Called every frame
void ARotatable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void ARotatable::StopPlayTimeLine()
{
	Timeline.Stop();
}

void ARotatable::HandleProgress(float Value)
{
	FRotator NewRotation = FMath::Lerp(ActorInitialRotation, TargetRotation, Value);
	SetActorRotation(NewRotation);
}

void ARotatable::OnAnimTriggered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		ARNB_KUNCharacter* RNB_KUNCharacter = Cast<ARNB_KUNCharacter>(OtherActor);
		if (RNB_KUNCharacter)
		{
			if ( !Timeline.IsPlaying() )
			{
				Timeline.PlayFromStart();
				FTimerHandle StopTimeLineTimer;
				GetWorldTimerManager().SetTimer(StopTimeLineTimer, this, &ARotatable::StopPlayTimeLine, TimeToPlay, false);
			}
		}
	}
}

