// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MyFire.generated.h"

class ARNB_KUNCharacter;

UCLASS()
class RNB_KUN_API AMyFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyFire();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere)
	UShapeComponent* CollisionBox;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* ParticleSystemComp;

	UPROPERTY()
	ARNB_KUNCharacter* CurrentPlayer;

	UPROPERTY(EditAnywhere)
	FString HelpText = FString(TEXT("Press F to activate the fire!"));

	int32 ResetTime = 10; // sec

	bool bIsPlayerInRange = false;

	bool bIsFireLit = false;

	void GetPlayer(AActor* Player);

	void Light();

	void AdvanceTimer();

	void TimerHasFinished();

	FTimerHandle CountdownTimerHandle;

	UFUNCTION()
	void TriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void TriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
