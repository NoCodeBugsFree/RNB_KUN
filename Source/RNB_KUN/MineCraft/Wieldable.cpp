// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "Wieldable.h"
#include "MineCraftCharacter.h"


// Sets default values
AWieldable::AWieldable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WieldableMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Wieldable Mesh"));
	SetRootComponent(WieldableMeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AWieldable::OnOverlapBegin);

	RotationMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotation Movement"));
	RotationMovement->RotationRate = FRotator(0.f, 180.f, 0.f);

	bIsActive = true;
}

// Called when the game starts or when spawned
void AWieldable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWieldable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWieldable::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && bIsActive)
	{
		AMineCraftCharacter* MineCraftCharacter = Cast<AMineCraftCharacter>(OtherActor);
		if (MineCraftCharacter)
		{
			MineCraftCharacter->GetFP_WeildedItem()->SetSkeletalMesh(WieldableMeshComponent->SkeletalMesh);
			Hide(true);
			MineCraftCharacter->AddItemToInventory(this);
			MineCraftCharacter->SetMaterialType(MaterialType);
			MineCraftCharacter->SetToolType(ToolType);
		}
	}	
}

void AWieldable::Hide(bool bShouldBeHidden)
{
	WieldableMeshComponent->SetVisibility(!bShouldBeHidden);
	bIsActive = !bShouldBeHidden;
}

