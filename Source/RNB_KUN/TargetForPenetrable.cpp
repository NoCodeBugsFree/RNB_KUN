// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "TargetForPenetrable.h"
#include "RNB_KUNCharacter.h"


// Sets default values
ATargetForPenetrable::ATargetForPenetrable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target Mesh"));
	RootComponent = TargetMesh;

	HealthText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Health Text"));
	HealthText->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATargetForPenetrable::BeginPlay()
{
	Super::BeginPlay();

	/** initialize help text  */
	HealthText->SetText(FString::Printf(TEXT("Health: %d"), Health));
}

// Called every frame
void ATargetForPenetrable::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	MakeTextFacePlayer();
}

void ATargetForPenetrable::DamageTarget(int32 Damage)
{
	Health -= Damage;

	if (Health <= 0)
	{
		OnDestroy();
	}
	else
	{
		HealthText->SetText(FString::Printf(TEXT("Health: %d"), Health));
	}
}

void ATargetForPenetrable::OnDestroy()
{
	Destroy();
}

void ATargetForPenetrable::MakeTextFacePlayer()
{
	if (HealthText)
	{
		ARNB_KUNCharacter* Character = Cast<ARNB_KUNCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (Character)
		{
			FRotator PlayerRotation = Character->GetActorRotation();
			PlayerRotation.Yaw += 180;
			HealthText->SetRelativeRotation(PlayerRotation);
		}
	}
}

