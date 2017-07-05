// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "Projectile.h"
#include "Sniper/SniperTarget.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollision"));
	ProjectileCollision->InitBoxExtent(FVector(2.f));
	ProjectileCollision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	ProjectileCollision->BodyInstance.SetCollisionProfileName("Projectile");
	SetRootComponent(ProjectileCollision);
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = ProjectileCollision;
	ProjectileMovement->InitialSpeed = ProjectileSpeed;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // will be applied by timer later

	ProjectileParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileParticles"));
	ProjectileParticles->SetupAttachment(RootComponent);

	InitialLifeSpan = 3.f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	/** apply gravity by timer  */
	FTimerHandle ApplyGravityTimer;
	GetWorldTimerManager().SetTimer(ApplyGravityTimer, this, &AProjectile::ApplyGravity, GravityDelay, false);
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{	
		if (Hit.BoneName != NAME_None)
		{
			UE_LOG(LogTemp, Error, TEXT("hit %s"), *Hit.BoneName.ToString());
		}

		if (!OtherActor->IsPendingKill())
		{
			OtherActor->Destroy();
		}
		

		if (!IsPendingKill())
		{
			Destroy();
		}
	}
}

void AProjectile::ApplyGravity()
{
	ProjectileMovement->ProjectileGravityScale = GravityScale;
}

