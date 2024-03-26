// Copyright Vaesty


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	// ensures the player controller is replicated across network
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	LastActor = CurrentActor;
	CurrentActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	// Scenario A: LastActor is null && CurrentActor is null
	// This scenario is implicitly handled by the early return if there's no blocking hit

	// Scenario B: LastActor is null && CurrentActor is valid
	if (LastActor == nullptr && CurrentActor != nullptr)
	{
		CurrentActor->HighlightActor(); // Highlight CurrentActor
	}
	// Scenario C: LastActor is valid && CurrentActor is null
	else if (LastActor != nullptr && CurrentActor == nullptr)
	{
		LastActor->UnhighlightActor(); // Unhighlight LastActor
	}
	// Scenario D: Both actors are valid, but LastActor != CurrentActor
	else if (LastActor != nullptr && CurrentActor != nullptr && LastActor != CurrentActor)
	{
		LastActor->UnhighlightActor(); // Unhighlight LastActor
		CurrentActor->HighlightActor(); // Highlight CurrentActor
	}
	// Scenario E: Both actors are valid, and LastActor == CurrentActor
	// - Do nothing
	else if (LastActor != nullptr && CurrentActor != nullptr && LastActor == CurrentActor)
	{
		// Explicitly handling this case helps clarify intentions,
		// even though no action is taken.
	}
}


void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// ensure the input mapping context is valid
	check(AuraContext);

	// Retrieves the local players Enhanced Input Subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	// Ensure the subsystem is valid
	check(Subsystem);
	// Adds our custom input mapping context with priority 0s
	Subsystem->AddMappingContext(AuraContext,0);
	// Allows the player to see their mouse cursor
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// Configures the game to both accept UI and game inputs, without locking the mouse to the viewport
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

// Sets up player input bindings using the Enhanced Input system.
void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// Casts the InputComponent to an EnhancedInputComponent to access Enhanced Input functionalities.
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	// Binds the MoveAction to the Move function, triggered by player input.
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

// Handles player movement based on input values, translating them to movement in the game world.
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// Converts input values to a 2D vector for directional movement.
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	
	// Gets the player's current orientation to calculate forward and right directions.
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Applies the input as movement commands to the pawn, if one is controlled by this controller.
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
