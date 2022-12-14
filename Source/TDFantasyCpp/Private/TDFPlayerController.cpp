// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "TDFPlayerController.h"
#include "BaseChest.h"
#include "BaseEnemy.h"
#include "BasePlayableCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#pragma endregion


#pragma region Constructor
ATDFPlayerController::ATDFPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}
#pragma endregion


#pragma region Events
#pragma region Tick
void ATDFPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// Check important changes
	UpdateMouseDisplay();
	CheckMouseMovement();
	CheckCursorChange();
}
#pragma endregion

#pragma region Setup Input
void ATDFPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// On mouse click action bind
	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ATDFPlayerController::SendTo);
}
#pragma endregion
#pragma endregion

#pragma region Set Destination Methods
void ATDFPlayerController::SendTo()
{
	// Get cursor hit
	GetCursorHit();

	// Is is not an enemy
	if(bIsEnemy == false)
	{
		// Move player to location
		MoveToHitLocation(MoveToLocation);
	}

	// If is an enemy
	else
	{
		CallBasicAttack(EnemyReference);
	}
}
#pragma endregion

#pragma region Hit
void ATDFPlayerController::GetCursorHit()
{
	// Get hit result
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Camera, true, Hit);

	// Set variables
	MoveToLocation = Hit.Location;
	EnemyReference = nullptr;
	ChestReference = nullptr;
	bIsEnemy = false;
	bIsChest = false;

	// If didn't hit an actor, clear all stats and exit
	if(Hit.GetActor() == nullptr)
	{
		return;
	}

	// Try to cast to BaseEnemy class
	EnemyReference = Cast<ABaseEnemy>(Hit.GetActor());

	// If cast succeded, indicates that it is an enemy and exit
	if(EnemyReference)
	{
		bIsEnemy = true;
		return;
	}

	// Try to cast to BaseChest class
	ChestReference = Cast<ABaseChest>(Hit.GetActor());

	// If cast succeded, indicates that it is a chest and exit
	if(ChestReference)
	{
		bIsChest = true;
		return;
	}
}
#pragma endregion

#pragma region Movement
void ATDFPlayerController::MoveToHitLocation(FVector TargetLocation)
{
	// Simple move to TargetLocation
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, TargetLocation);

	// Spawn click FX cursor
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, TargetLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);

	// Get player
	ABasePlayableCharacter* BaseCharacter = Cast<ABasePlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Reset focus enemy and auto attack
	BaseCharacter->SetFocusEnemy(nullptr);
	BaseCharacter->ResetAutoAttack();
}
#pragma endregion

#pragma region Combat
void ATDFPlayerController::CallBasicAttack(ABaseEnemy* Enemy)
{
	// Stop player movement
	StopMovement();

	// Get player
	ABasePlayableCharacter* BaseCharacter = Cast<ABasePlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Call player's basic attack method
	BaseCharacter->BasicAttack(Enemy);
}
#pragma endregion


#pragma region Mode
void ATDFPlayerController::ChangeMode(int Option)
{
	// Setup Game and UI mode
	FInputModeGameAndUI GameAndUI = FInputModeGameAndUI();

	// Set Hide Cursor During Capture to false, so the mouse pointer image does not flicker
	GameAndUI.SetHideCursorDuringCapture(false);

	switch(Option)
	{
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

#pragma region Cursor Change
void ATDFPlayerController::UpdateMouseDisplay()
{
	// If player is using gamepad
	if(bIsUsingGamepad)
	{
		// Hide cursor and change mode to Game Only
		bShowMouseCursor = false;
		//ChangeMode(2);
	}
	// If not
	else
	{
		// Show cursor and change mode to Game and UI
		bShowMouseCursor = true;
		//ChangeMode(0);
	}
}

void ATDFPlayerController::CheckMouseMovement()
{
	// Mouse position local variables
	float _MouseX;
	float _MouseY;

	// Get mouse position
	GetMousePosition(_MouseX, _MouseY);

	// If mouse position is different than the last mouse position variable
	if(_MouseX != MouseX || _MouseY != MouseY)
	{
		// Change bUsingGamepad to false and mode to Game and UI
		bIsUsingGamepad = false;
		ChangeMode(0);
	}

	// Set mouse position variables
	MouseX = _MouseX;
	MouseY = _MouseY;
}

void ATDFPlayerController::CheckCursorChange()
{
	// Get cursor hit
	GetCursorHit();

	// If is enemy, change cursor type to attack
	if(bIsEnemy)
	{
		ChangeCursorImage(ECursorType::ECT_Attack);
	}
	// If is chest, change cursor type to information
	else if(bIsChest)
	{
		ChangeCursorImage(ECursorType::ECT_Information);
	}
	// If not, set to default cursor
	else
	{
		ChangeCursorImage(ECursorType::ECT_Default);
	}
}

void ATDFPlayerController::ChangeCursorImage(ECursorType CursorType)
{
	// If cursor type is default, set mouse cursor to default
	if(CursorType == ECursorType::ECT_Default)
	{
		CurrentMouseCursor = EMouseCursor::Default;
	}
	// If cursor type is attack, set mouse cursor to crosshairs
	else if(CursorType == ECursorType::ECT_Attack)
	{
		CurrentMouseCursor = EMouseCursor::Crosshairs;
	}
	// Else, set mouse cursor to eye dropper
	else
	{
		CurrentMouseCursor = EMouseCursor::EyeDropper;
	}


}
#pragma endregion

#pragma region Getters
bool ATDFPlayerController::GetIsUsingGamepad()
{
	return bIsUsingGamepad;
}
#pragma endregion
