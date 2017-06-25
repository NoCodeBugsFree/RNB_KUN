// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

UCLASS()
class RNB_KUN_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABlock();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	
	
	/* Represents block mesh  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SM_BlockMesh;

	/** Resistance to break events  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float Resistance = 20.f;

	/** Life periods of block when it is breaking  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float BreakingStages = 0.f;
	
	/** Minimum Material to break this block  */
	EMaterial MinimunMaterial = EMaterial::M_Wooden;

	/**  Dynamic Material for this block */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicMaterial;
	

private:
	
	/** Calls once the block has hit the final breaking stage  */
	void OnBroken(bool HasRequiredPickAxe);
	
public:

	/** Calls when player wants to break this actor  */
	void Break();
	
	/** Restore the material when character stopped breaking this block  */
	void ResetBlock();

	/** Returns the resistance for this type of block  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	float GetResistance() const { return Resistance; }

};
