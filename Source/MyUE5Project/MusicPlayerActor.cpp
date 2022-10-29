// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicPlayerActor.h"

#include "Components/AudioComponent.h"

// Sets default values
AMusicPlayerActor::AMusicPlayerActor()
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetAutoActivate(false);
	AudioComponent->SetSound(MusicCue);
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMusicPlayerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMusicPlayerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

