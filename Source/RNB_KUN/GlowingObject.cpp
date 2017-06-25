// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "GlowingObject.h"


// Sets default values
AGlowingObject::AGlowingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh Component"));
	RootComponent = StaticMeshComponent;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetupAttachment(RootComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AGlowingObject::EnteredObjectRadius);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AGlowingObject::LeftObjectRadius);
}

// Called when the game starts or when spawned
void AGlowingObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGlowingObject::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AGlowingObject::ToggleGlow(bool bIsGlowing)
{
	StaticMeshComponent->SetRenderCustomDepth(bIsGlowing);
	// glowing effect
	// unbound post process volume
	// blendables array->add->M_HiLight
}

void AGlowingObject::EnteredObjectRadius(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ToggleGlow(true);
}

void AGlowingObject::LeftObjectRadius(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ToggleGlow(false);
}

