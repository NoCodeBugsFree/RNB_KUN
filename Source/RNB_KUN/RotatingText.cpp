// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "RotatingText.h"
#include "RNB_KUNCharacter.h"


// Sets default values
ARotatingText::ARotatingText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	if (TextRenderComponent)
	{
		TextRenderComponent->SetWorldSize(70);
		TextRenderComponent->SetTextRenderColor(FColor::Red);
		TextRenderComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	}
}

// Called when the game starts or when spawned
void ARotatingText::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARotatingText::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	MakeTextFacePlayer();
}

void ARotatingText::MakeTextFacePlayer()
{
	if (TextRenderComponent)
	{
		ARNB_KUNCharacter* Character = Cast<ARNB_KUNCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (Character)
		{
			FRotator NewRot = Character->GetActorRotation();
			NewRot.Yaw += 180.f;
			TextRenderComponent->SetRelativeRotation(NewRot);
		}
	}
}

