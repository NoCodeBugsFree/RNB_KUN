// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "FollowerCharacter.h"
#include "FollowerAIController.h"
#include "RNB_KUNCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFollowerCharacter::AFollowerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AFollowerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AFollowerAIController* FollowerAIController = Cast<AFollowerAIController>(GetController());
	if (FollowerAIController)
	{
		ARNB_KUNCharacter* RNB_KUNCharacter = Cast<ARNB_KUNCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		FollowerAIController->SetPlayerToFollow(RNB_KUNCharacter);
	}

}

// Called every frame
void AFollowerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFollowerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

