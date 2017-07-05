// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "LauncherProjectile.h"
#include "PhysicsEngine/DestructibleActor.h"

ALauncherProjectile::ALauncherProjectile()
{
	LauncherProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LauncherProjectileMesh"));
	LauncherProjectileMesh->SetupAttachment(RootComponent);

	GetCollisionComp()->SetSphereRadius(20.f);

	bIsDetonated = false;
}


void ALauncherProjectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ALauncherProjectile::OnDetonate, TimeToExplode, false);
}

void ALauncherProjectile::OnDetonate()
{
	if (ExplosionParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorTransform());
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}

	TArray<FHitResult> HitResults;
	FVector StartTrace = GetActorLocation();
	FVector EndTrace = StartTrace;
	EndTrace.Z += 300.f;

	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(ExplosionRadius);

	if (GetWorld()->SweepMultiByChannel(HitResults, StartTrace, EndTrace, FQuat::FQuat(), ECC_WorldStatic, CollisionShape))
	{
		for (FHitResult& Hit : HitResults)
		{
			UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Hit.Actor->GetRootComponent());
			ADestructibleActor* DestructibleActor = Cast<ADestructibleActor>(Hit.GetActor());

			if (StaticMeshComponent)
			{
				StaticMeshComponent->AddRadialImpulse(GetActorLocation(), 1000.f, 5000.f, ERadialImpulseFalloff::RIF_Linear, true);
			} 
			else if(DestructibleActor)
			{
				DestructibleActor->GetDestructibleComponent()->ApplyRadiusDamage(10.f, Hit.ImpactPoint, 500.f, 3000.f, false);
			}
		}
	}
	
	Destroy();
}

void ALauncherProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && !bIsDetonated)
	{
		bIsDetonated = true;
		OnDetonate();
	}
}
