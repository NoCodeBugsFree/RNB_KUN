// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "PenetrableMaterial.h"


// Sets default values
APenetrableMaterial::APenetrableMaterial()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaterialMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Materia lMesh"));
	RootComponent = MaterialMesh;
}

// Called when the game starts or when spawned
void APenetrableMaterial::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APenetrableMaterial::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

