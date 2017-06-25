// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class RNB_KUN_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AInteractable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** TODO AActor* Caller and remove all redundant casts  */
	/** Generic interaction function for all interactables  */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AAA")
	void Interact();
	virtual void Interact_Implementation();

protected:

	/** Represents interactable actor mesh   */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* InteractableMesh;
	
	/** Help text which appears when character is close to interactable  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	FString InteractableHeplText;

public:

	/** Returns Interactable Mesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetInteractableMesh() const { return InteractableMesh; }

	/** Returns Interactable Hepl Text **/
	FORCEINLINE FString GetHeplText() const { return InteractableHeplText; }
};
