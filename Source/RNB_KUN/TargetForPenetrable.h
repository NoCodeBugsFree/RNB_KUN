// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TargetForPenetrable.generated.h"

UCLASS()
class RNB_KUN_API ATargetForPenetrable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetForPenetrable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** Handles target's damaging  */
	void DamageTarget(int32 Damage);

private:

	/** Health Text */
	UPROPERTY(EditAnywhere)
	UTextRenderComponent* HealthText;

	/** Target's mesh  */
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TargetMesh;

	/** Health Amount */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	int32 Health = 100;
	
	/** Simple destroy when health less 0  */
	void OnDestroy();

	/** Rotate text to character per tick  */
	void MakeTextFacePlayer();
};
