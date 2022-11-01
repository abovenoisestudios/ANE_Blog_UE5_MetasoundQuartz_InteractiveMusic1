// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Quartz/AudioMixerClockHandle.h" //Include this directive to access the Quartz clock.
#include "MetasoundSource.h" //Include this directive to use MetasoundSource properties.

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

	#pragma region PROPERTIES

	/** Enables on screen messages for debug. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	bool bDebug = false;

	/** Metasound music cue. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Music")
	UMetaSoundSource* MusicCue;

	/** Audio component that will hold and control the Metasound music cue. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Music")
	UAudioComponent* MusicAudioComponent;

	/** Current trigger parameter name. It changes and updates the music "states" on the Metasound. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Music")
	FName TriggerParameter;

	/** Quartz clock. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quartz Clock")
	UQuartzClockHandle* MusicClock;

	/** Name for the Quartz music clock. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quartz Clock")
	FName NewClockName = "MusicClock";

	/** Music meter numerator. Default value = 4. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quartz Clock")
	int MeterNumerator = 4;

	/** Music meter denominator enumeration. Default value = QuarterNote. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quartz Clock")
	EQuartzTimeSignatureQuantization MeterDenominator = EQuartzTimeSignatureQuantization::QuarterNote;

	/** Beats per minute to set for the Quartz clock. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quartz Clock")
	float BPM = 76.0f;

	/** Number of bars for the MusicCue loop. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quartz Clock")
	int BarNumbers = 8;

	/** Defines the musical timing boundary when the functions will trigger.
	 *  As default, this enumeration will make the functions trigger at the end of the beat. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quartz Clock")
	EQuartzCommandQuantization QuantizationBoundary = EQuartzCommandQuantization::Beat;

	#pragma endregion PROPERTIES

	#pragma region FUNCTIONS AND DELEGATES

	/**
	 * @brief Sets and creates a new Quartz clock and starts the music cue quantized.
	 * @param ClockName Name for the new Quartz clock.
	 * @param NumBeats Meter numerator.
	 * @param BeatType Meter denominator.
	 * @param BeatsPerMinute Beats per minute / tempo.
	 * @param AudioComponent Music audio component.
	 */
	UFUNCTION(BlueprintCallable, Category="Music")
	void CreateClockAndPlayMusic(FName ClockName, int NumBeats, EQuartzTimeSignatureQuantization BeatType, float BeatsPerMinute, UAudioComponent* AudioComponent);

	/**
	 * @brief Function delegate. Triggers its functionality synced with the clock.
	 * @param EventType Use a switch on this enumeration to select "CommandOnQueued". 
	 * @param Name 
	 */
	UFUNCTION()
	void FPlayQuantizedDelegate(EQuartzCommandDelegateSubType EventType, FName Name);

	/** Used to bind the FPlayQuantizedDelegate in the class constructor. */
	FOnQuartzCommandEventBP PlayQuantizationDelegate;

	
	/**
	 * @brief Executes a trigger parameter synced with the clock.
	 * @param Clock Quartz clock.
	 * @param InQuantizationBoundary Quantization boundary.
	 * @param ParameterName Parameter to be triggered on the Music Cue.
	 */
	UFUNCTION(BlueprintCallable, Category="Music")
	void ExecuteTriggerInTime(UQuartzClockHandle* Clock, EQuartzCommandQuantization InQuantizationBoundary, FName ParameterName);

	/**
	 * @brief Function delegate. Triggers its functionality synced with the clock.
	 * @param ClockName Name of the Quartz clock.
	 * @param QuantizationType 
	 * @param NumBars 
	 * @param Beat 
	 * @param BeatFraction 
	 */
	UFUNCTION()
	void FExecuteTriggerDelegate(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars, int32 Beat, float BeatFraction);

	/** Used to bind the FPlayQuantizedDelegate in the class constructor. */
	FOnQuartzMetronomeEventBP ExecuteTriggerDelegate;

	
	/**
	 * @brief Resets the clock at the last bar of the music loop.
	 * @param Clock Quartz clock.
	 * @param InQuantizationBoundary Quantization boundary.
	 * @param NumBars Number of bars for the MusicCue loop.
	 */
	UFUNCTION(BlueprintCallable, Category="Music")
	void UpdateAndResetClock(UQuartzClockHandle* Clock, EQuartzCommandQuantization InQuantizationBoundary, int NumBars);

	/**
	 * @brief Function delegate. Triggers its functionality synced with the clock.
	 * @param ClockName Name of the Quartz clock.
	 * @param QuantizationType 
	 * @param NumBars 
	 * @param Beat 
	 * @param BeatFraction 
	 */
	UFUNCTION()
	void FUpdateClockDelegate(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars, int32 Beat, float BeatFraction);

	/** Used to bind the FPlayQuantizedDelegate in the class constructor. */
	FOnQuartzMetronomeEventBP UpdateClockDelegate;

	
	#pragma endregion FUNCTIONS AND DELEGATES

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
