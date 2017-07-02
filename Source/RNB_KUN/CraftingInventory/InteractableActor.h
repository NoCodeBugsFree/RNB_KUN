// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class RNB_KUN_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AInteractableActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Calls when interaction happens  */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void Interact(class APlayerController* PlayerController);

protected:

	/** Actor's name  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	FString Name = "Name";

	/** Actor's action name   */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	FString Action = "Action";

	/** Returns the name of this actor and its action for represent this data on the HUD  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	FString GetUseText() const { return FString::Printf(TEXT("%s : Press E to %s"), *Name, *Action); }
	
};
