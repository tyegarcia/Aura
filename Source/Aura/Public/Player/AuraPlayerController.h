// Copyright Vaesty

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * Custom PlayerController for the Aura project. Handles player input and interactions.
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Constructor for AAuraPlayerController.
	AAuraPlayerController();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;
	// Sets up the player's input component.
	virtual void SetupInputComponent() override;

private:
	// Reference to the input mapping context used for managing input mappings.
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	// Reference to the input action for moving the player.
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	// Function to handle movement based on input action values.
	void Move(const struct FInputActionValue& InputActionValue);
};
