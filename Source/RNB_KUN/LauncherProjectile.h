// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RNB_KUNProjectile.h"
#include "LauncherProjectile.generated.h"

/**
 * 
 */
UCLASS()
class RNB_KUN_API ALauncherProjectile : public ARNB_KUNProjectile
{
	GENERATED_BODY()
	
public:

	ALauncherProjectile();

	virtual void BeginPlay() override;

protected:

	/** Detonate once  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	uint32 bIsDetonated : 1;

	/* Explosion Particles */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ExplosionParticles;

	/** Explosion Sound  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class USoundCue* ExplosionSound;

	/** Explosion Radius  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float ExplosionRadius = 500.f;

	/** Time To Explode  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float TimeToExplode = 5.f;

	/** Calls when timer expired  */
	UFUNCTION()
	void OnDetonate();

	/** Calls when this actor hits something  */
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	/* Launcher Projectile Mesh  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LauncherProjectileMesh;
	
};
