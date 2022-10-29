// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MetasoundSource.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MusicPlayerActor.generated.h"

UCLASS()
class MYUE5PROJECT_API AMusicPlayerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMusicPlayerActor();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	UMetaSoundSource* MusicCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Audio")
	UAudioComponent* AudioComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
