// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "Bullet.h"
#include "PhysicsEngine/DestructibleActor.h"


// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneRoot);

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult HitResult;

	FVector StartTrace = GetActorLocation();
	FVector EndTrace = StartTrace + Velocity * DeltaTime;
	EndTrace.Z = GetActorRotation().Pitch;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Destructible, CollisionQueryParams))
	{
		if (HitResult.GetActor())
		{
			DrawDebugSolidBox(GetWorld(), HitResult.ImpactPoint, FVector(10.f), FColor::Blue, true);
			ADestructibleActor* DestructibleActor = Cast<ADestructibleActor>(HitResult.GetActor());

			if (DestructibleActor)
			{
				DestructibleActor->GetDestructibleComponent()->ApplyRadiusDamage(10.f, HitResult.ImpactPoint, 32.f, 10.f, false);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Couldn't get the mesh. Type is %s"), *HitResult.GetActor()->StaticClass()->GetName());
		}
		Destroy();
	}
	else
	{
		BulletExpiry += DeltaTime;
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(0.f, -BulletExpiry * 80.f, 100.f), true);
		SetActorLocation(EndTrace);
		Velocity += FVector(0.f, 0.f, -200.f) * DeltaTime;
	}

	if (BulletExpiry > 3)
	{
		Destroy();
	}

}

