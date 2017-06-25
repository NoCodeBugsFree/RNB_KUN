// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "MyFire.h"
#include "Engine.h"
#include "RNB_KUNCharacter.h"


// Sets default values
AMyFire::AMyFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;
	CollisionBox->bGenerateOverlapEvents = true;
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyFire::TriggerEnter);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AMyFire::TriggerExit);
	CollisionBox->SetRelativeScale3D(FVector(3.f, 3.f, 1.f));
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComp"));
	ParticleSystemComp->SetupAttachment(RootComponent);
	ParticleSystemComp->SetVisibility(false);
}

// Called when the game starts or when spawned
void AMyFire::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AMyFire::AdvanceTimer, 1.f, true);
	ParticleSystemComp->SetVisibility(false);
}

// Called every frame
void AMyFire::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (CurrentPlayer)
	{
		if (CurrentPlayer->IsInteracting() && bIsPlayerInRange && CurrentPlayer->Inventory.Contains("Matches") && !bIsFireLit)
		{
			Light();
		}
	}
}

void AMyFire::GetPlayer(AActor * Player)
{
	if (Player)
	{
		CurrentPlayer = Cast<ARNB_KUNCharacter>(Player);
	}
}

void AMyFire::Light()
{
	int32 XpGained = FMath::RandRange(10, 100);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (FString::Printf(TEXT("Light the fire and gain XP : %d"), XpGained)));
	if (CurrentPlayer)
	{
		CurrentPlayer->Inventory.RemoveSingle("Matches");
	}
	bIsFireLit = true;
	ParticleSystemComp->SetVisibility(true);
}

void AMyFire::AdvanceTimer()
{
	if (bIsFireLit)
	{
		--ResetTime;
		if (ResetTime < 1)
		{
			TimerHasFinished();
		}
	}
}

void AMyFire::TimerHasFinished()
{
	ParticleSystemComp->SetVisibility(false);
	bIsFireLit = false;
	ResetTime = 10;
}

void AMyFire::TriggerEnter(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	GetPlayer(OtherActor);
	bIsPlayerInRange = true;

	if (CurrentPlayer)
	{
		if (CurrentPlayer->Inventory.Contains("Matches"))
		{
			if (!bIsFireLit)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HelpText);
			}
		}
		else
		{
			if (!bIsFireLit)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("You need MATHCES...")));
			}
		}
	}
}

void AMyFire::TriggerExit(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("TriggerExit...")));
	bIsPlayerInRange = false;
}

