// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rotatable.generated.h"

UENUM(BlueprintType)
enum class ERotationAxis : uint8
{
	RA_Yaw UMETA(DisplayName = "Yaw"),
	RA_Pitch UMETA(DisplayName = "Pitch"),
	RA_Roll UMETA(DisplayName = "Roll")
};

UCLASS()
class RNB_KUN_API ARotatable : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ARotatable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	
	/** Specifies the axis to rotate about  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	ERotationAxis RotationAxis = ERotationAxis::RA_Yaw;
	
	/* Static Rotatable Mesh Component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RotatableMesh;
	
	/* Simple collision box  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;
	
	/** Rotation angle  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float MaxRotation = 90.f;
	
	/** Curve Float asset Miscellaneous - Curve */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* AnimCurve;

	UPROPERTY()
	FTimeline Timeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float TimeToPlay = 3.f;

	void StopPlayTimeLine();

	UFUNCTION()
	void HandleProgress(float Value);

	UFUNCTION()
	void OnAnimTriggered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:

	FRotator ActorInitialRotation;

	FRotator TargetRotation;
};
