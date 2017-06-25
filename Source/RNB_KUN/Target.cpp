// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "Target.h"


// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATarget::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATarget::OnDestroy()
{
	Destroy();
}

void ATarget::DamageTarget(float Damage)
{
	Health -= Damage;
	if (Health <= 0)
	{
		OnDestroy();
	}
}

