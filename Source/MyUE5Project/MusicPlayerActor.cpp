// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicPlayerActor.h"

#include "Components/AudioComponent.h" //Include this directive to use AudioComponents.

// Sets default values
AMusicPlayerActor::AMusicPlayerActor()
{
	/** Creates an audio component. */
	MusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	MusicAudioComponent->SetAutoActivate(false);
	MusicAudioComponent->SetSound(MusicCue);

	/** Binds all the function delegates. */
	PlayQuantizationDelegate.BindUFunction(this, "FPlayQuantizedDelegate");
	ExecuteTriggerDelegate.BindUFunction(this, "FExecuteTriggerDelegate");
	UpdateClockDelegate.BindUFunction(this, "FUpdateClockDelegate");

	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMusicPlayerActor::CreateClockAndPlayMusic(FName ClockName, int NumBeats, EQuartzTimeSignatureQuantization BeatType, float BeatsPerMinute, UAudioComponent* AudioComponent)
{
	/** Gets a reference from the Quartz subsystem from the world. */
	UQuartzSubsystem* Quartz = GetWorld()->GetSubsystem<UQuartzSubsystem>();

	/** Defines settings for FQuartzTimeSignature and FQuartzClockSettings structures. */
	FQuartzTimeSignature TimeSignature;
	TimeSignature.NumBeats = NumBeats;
	TimeSignature.BeatType = BeatType;
	FQuartzClockSettings ClockSettings;
	ClockSettings.TimeSignature = TimeSignature;

	/** Creates a new clock the previous setting structures. */
	MusicClock = Quartz->CreateNewClock(this, ClockName, ClockSettings, true);
	/** Sets the tempo for the clock. */
	MusicClock->SetBeatsPerMinute(this, FQuartzQuantizationBoundary(), FOnQuartzCommandEventBP(), MusicClock, BeatsPerMinute);

	/** Defines settings for the FQuartzQuantizationBoundary structure. */
	FQuartzQuantizationBoundary QuartzQuantizationBoundary;
	QuartzQuantizationBoundary.Quantization = EQuartzCommandQuantization::Bar;
	QuartzQuantizationBoundary.Multiplier = 1.0f;
	QuartzQuantizationBoundary.CountingReferencePoint = EQuarztQuantizationReference::BarRelative;
	QuartzQuantizationBoundary.bResetClockOnQueued = true;

	/** Plays the music cue from the audio component after the audio is "Queued" and loaded. Starts the clock. */
	AudioComponent->PlayQuantized(this, MusicClock, QuartzQuantizationBoundary, PlayQuantizationDelegate);
}

void AMusicPlayerActor::FPlayQuantizedDelegate(EQuartzCommandDelegateSubType EventType, FName Name)
{
	/** Breaks this enumeration on different execution paths. */
	switch (EventType)
	{
	/** Starts the clock after the audio cue is ready or "Queued" */
	case EQuartzCommandDelegateSubType::CommandOnQueued:
		MusicClock->StartClock(this, MusicClock);
		break;
		
	default:
		break;
	}

	if(bDebug) /** if bDebug is true, adds these messages to the screen. */
	{
		GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Green, "Music Start");
		GEngine->AddOnScreenDebugMessage(4, 5.0f, FColor::Red, "C++ Implementation");
	}
}

void AMusicPlayerActor::ExecuteTriggerInTime(UQuartzClockHandle* Clock, EQuartzCommandQuantization InQuantizationBoundary, FName ParameterName)
{
	/** Gets a copy of the new trigger parameter. */
	TriggerParameter = ParameterName;
	
	/** Creates a delegate that will trigger synced with the clock. */
	Clock->SubscribeToQuantizationEvent(this, InQuantizationBoundary, ExecuteTriggerDelegate, Clock);
}

void AMusicPlayerActor::FExecuteTriggerDelegate(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars, int32 Beat, float BeatFraction)
{
	/** Sets the new parameter in time. */
	MusicAudioComponent->SetTriggerParameter(TriggerParameter);
	/** Unsubscribe from the delegate so the parameter trigger once. */
	MusicClock->UnsubscribeFromTimeDivision(this, QuantizationBoundary, MusicClock);

	if (bDebug) /** if bDebug is true, adds these messages to the screen. */
	{
		GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Magenta, TriggerParameter.ToString());
		GEngine->AddOnScreenDebugMessage(4, 5.0f, FColor::Red, "C++ Implementation");
	}
}

void AMusicPlayerActor::UpdateAndResetClock(UQuartzClockHandle* Clock, EQuartzCommandQuantization InQuantizationBoundary, int NumBars)
{
	/** Creates a delegate that will trigger synced with the clock. */
	Clock->SubscribeToQuantizationEvent(this, EQuartzCommandQuantization::EighthNote, UpdateClockDelegate, Clock);
}

void AMusicPlayerActor::FUpdateClockDelegate(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars, int32 Beat, float BeatFraction)
{
	/** Defines settings for the FQuartzQuantizationBoundary structure. */
	FQuartzQuantizationBoundary QuartzQuantizationBoundary;
	QuartzQuantizationBoundary.Quantization = QuantizationBoundary;
	QuartzQuantizationBoundary.Multiplier = 1.0f;
	QuartzQuantizationBoundary.CountingReferencePoint = EQuarztQuantizationReference::BarRelative;

	/** If the current bar position is equal to the music cue lenght, reset the clock at the end of that boundary.  */
	if(NumBars == BarNumbers)
		MusicClock->ResetTransportQuantized(this, QuantizationBoundary, FOnQuartzCommandEventBP(), MusicClock);

	/** if bDebug is true, adds these messages to the screen. */
	if(bDebug)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Orange, "Bar: " + FString::FromInt(NumBars));
		GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Purple, "Beat: " + FString::FromInt(Beat));
	}
}

// Called when the game starts or when spawned
void AMusicPlayerActor::BeginPlay()
{
	Super::BeginPlay();

	
	//MusicClock->SubscribeToQuantizationEvent(this, EQuartzCommandQuantization::Beat, CommandEvent, MusicClock);
}

// Called every frame
void AMusicPlayerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

