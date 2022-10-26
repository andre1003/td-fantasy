// Copyright Epic Games, Inc. All Rights Reserved.

#pragma region Includes
#include "TDFantasyCppPlayerController.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "TDFantasyCppCharacter.h"
#include "Engine/World.h"
#include "BaseEnemy.h"
#pragma endregion


#pragma region Constructor
ATDFantasyCppPlayerController::ATDFantasyCppPlayerController() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}
#pragma endregion

#pragma region Events
#pragma region Tick
void ATDFantasyCppPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	// Check important changes
	UpdateMouseDisplay();
	CheckMouseMovement();
	CheckCursorChange();

}
#pragma endregion

#pragma region Setup Input
void ATDFantasyCppPlayerController::SetupInputComponent() {
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ATDFantasyCppPlayerController::OnSetDestinationPressed);
	//InputComponent->BindAction("SetDestination", IE_Released, this, &ATDFantasyCppPlayerController::OnSetDestinationReleased);
}
#pragma endregion
#pragma endregion

#pragma region Set Destination Methods
void ATDFantasyCppPlayerController::OnSetDestinationPressed() {
	//// We flag that the input is being pressed
	//bInputPressed = true;
	//// Just in case the character was moving because of a previous short press we stop it
	//StopMovement();

	//// If it was a short press
	//if(FollowTime <= ShortPressThreshold) {
	//	// We look for the location in the world where the player has pressed the input
	//	FVector HitLocation = FVector::ZeroVector;
	//	FHitResult Hit;
	//	GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	//	HitLocation = Hit.Location;

	//	// We move there and spawn some particles
	//	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitLocation);
	//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, HitLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	//}

	// Get cursor hit
	GetCursorHit();

	// Is is not an enemy
	if(bIsEnemy == false)
		// Move player to location
		MoveToHitLocation(MoveToLocation);

}

void ATDFantasyCppPlayerController::OnSetDestinationReleased() {
	// Player is no longer pressing the input
	bInputPressed = false;

	// If it was a short press
	if(FollowTime <= ShortPressThreshold) {
		// We look for the location in the world where the player has pressed the input
		FVector HitLocation = FVector::ZeroVector;
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		HitLocation = Hit.Location;

		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitLocation);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, HitLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
}
#pragma endregion

#pragma region Hit
void ATDFantasyCppPlayerController::GetCursorHit() {
	// Get hit result
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Camera, true, Hit);

	// Set variables
	MoveToLocation = Hit.Location;

	// If hit an actor
	if(Hit.GetActor() != nullptr) {
		// Try to cast to BaseEnemy class
		EnemyReference = Cast<class ABaseEnemy>(Hit.GetActor());

		// If cast succeded
		if(EnemyReference) {
			bIsEnemy = true;
			bIsChest = false;
		}
		// If not
		else {
			EnemyReference = nullptr;
			bIsEnemy = false;
		}
	}
	// If didn't hit an actor, clear all stats
	else {
		EnemyReference = nullptr;
		bIsEnemy = false;
		bIsChest = false;
	}
}
#pragma endregion

#pragma region Movement
void ATDFantasyCppPlayerController::MoveToHitLocation(FVector TargetLocation) {
	// Simple move to TargetLocation
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, TargetLocation);

	// Spawn click FX cursor
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, TargetLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
}
#pragma endregion

#pragma region Mode
void ATDFantasyCppPlayerController::ChangeMode(int Option) {
	// Setup Game and UI mode
	FInputModeGameAndUI GameAndUI = FInputModeGameAndUI();

	// Set Hide Cursor During Capture to false, so the mouse pointer image does not flicker
	GameAndUI.SetHideCursorDuringCapture(false);

	switch(Option) {
	// Game and UI
	case 0:
		SetInputMode(GameAndUI);
		break;

	// UI only
	case 1:
		SetInputMode(FInputModeUIOnly());
		break;

	// Game only
	case 2:
		SetInputMode(FInputModeGameOnly());
		break;
	}
}
#pragma endregion

#pragma region Check
void ATDFantasyCppPlayerController::UpdateMouseDisplay() {
	// If player is using gamepad
	if(bIsUsingGamepad) {
		// Hide cursor and change mode to Game Only
		bShowMouseCursor = false;
		ChangeMode(2);
	}
	// If not
	else {
		// Show cursor and change mode to Game and UI
		bShowMouseCursor = true;
		ChangeMode(0);
	}
}

void ATDFantasyCppPlayerController::CheckMouseMovement() {
	// Mouse position local variables
	float _MouseX;
	float _MouseY;

	// Get mouse position
	GetMousePosition(_MouseX, _MouseY);

	// If mouse position is different than the last mouse position variable
	if(_MouseX != MouseX || _MouseY != MouseY) {
		// Change bUsingGamepad to false and mode to Game and UI
		bIsUsingGamepad = false;
		ChangeMode(0);
	}

	// Set mouse position variables
	MouseX = _MouseX;
	MouseY = _MouseY;
}
#pragma endregion
