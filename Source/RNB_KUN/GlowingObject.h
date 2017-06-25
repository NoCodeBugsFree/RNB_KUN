// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GlowingObject.generated.h"

UCLASS()
class RNB_KUN_API AGlowingObject : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AGlowingObject();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	/** enable/disable glowing effect  */
	void ToggleGlow(bool bIsGlowing);

	/** calls when character enters the collision box  */
	UFUNCTION()
	void EnteredObjectRadius(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** calls when character leaves the collision box  */
	UFUNCTION()
	void LeftObjectRadius(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	
	/** Represents the mesh of the item  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;
	
	/** Collision box  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;
	
};
