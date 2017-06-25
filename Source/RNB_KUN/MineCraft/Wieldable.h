// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wieldable.generated.h"

UCLASS()
class RNB_KUN_API AWieldable : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AWieldable();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Calls when SphereComponent overlaps with another actor  */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	/** Type of this tool  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	ETool ToolType = ETool::E_Unarmed;

	/** Type of material for this tool  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	EMaterial MaterialType = EMaterial::M_None;

private:

	/**  Shows whether or not this actor is active  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	uint32 bIsActive : 1;

	/** Simple picture for this item to show it in HUD  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UTexture2D* WieldableThumbnail;

	/** Rotating Movement Component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	URotatingMovementComponent* RotationMovement;

	/* Collision sphere for this actor  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;

	/* Represents Wieldable Mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WieldableMeshComponent;
	
public:
	
	/** Returns Thumbnail for the HUD  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	UTexture2D* GetThumbnail() const { return WieldableThumbnail; }
	
	/** Returns Wieldable Mesh Component subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetWieldableMesh() const { return WieldableMeshComponent; }
	
	/** Hides and disable Wieldable Item  */
	void Hide(bool bShouldBeHidden);
};
