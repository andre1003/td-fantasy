// Copyright Epic Games, Inc. All Rights Reserved.
// Edit by Paulo André "Dufwine" Pimenta Aragão.

#pragma once

#pragma region Includes
#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "Data.h"
#include "BasePlayerController.generated.h"
#pragma endregion


/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class ABasePlayerController : public APlayerController
{
	GENERATED_BODY()


#pragma region Attributes
	#pragma region Public Attributes
public:
	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UNiagaraSystem* FXCursor;
	#pragma endregion

	#pragma region Protected Attributes
protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Is player using gamepad controller
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gamepad")
		bool bIsUsingGamepad = false;

	// Is cursor on enemy controller
	UPROPERTY(BlueprintReadOnly)
		bool bIsEnemy = false;

	// Is chest controller
	UPROPERTY(BlueprintReadOnly)
		bool bIsChest = false;


	// Cursor enemy reference
	UPROPERTY(BlueprintReadOnly)
		class ABaseEnemy* EnemyReference;

	// Cursor chest reference
	UPROPERTY(BlueprintReadOnly)
		class ABaseChest* ChestReference;
	#pragma endregion

	#pragma region Private Attributes
private:
	bool bInputPressed; // Input is bring pressed
	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed

	// Clicked location to move
	FVector MoveToLocation;

	// Mouse axis
	float MouseX;
	float MouseY;
	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	ABasePlayerController();

	#pragma region Mode
	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Change controller mode.
		/// </summary>
		/// <param name="Option">Controller mode option.</param>
		void ChangeMode(int Option);
	#pragma endregion

	#pragma region Cursor Change
	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Check for cursor image change.
		/// </summary>
		void CheckCursorChange();

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Change cursor image, based on parameter.
		/// </summary>
		/// <param name="CursorType">Cursor type option.</param>
		void ChangeCursorImage(ECursorType CursorType);
	#pragma endregion

	/// <summary>
	/// Get bIsUsingGamepad varible value.
	/// </summary>
	/// <returns>bIsUsingGamepad value</returns>
	bool GetIsUsingGamepad();
	#pragma endregion

	#pragma region Protected Methods
protected:
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
	#pragma endregion

	#pragma region Private Methods
private:
	#pragma region Hit
	/// <summary>
	/// Get hit variables under cursor.
	/// </summary>
	void GetCursorHit();
	#pragma endregion

	#pragma region Movement
	/// <summary>
	/// Move player to location.
	/// </summary>
	/// <param name="TargetLocation">Target location to move.</param>
	void MoveToHitLocation(FVector TargetLocation);
	#pragma endregion

	#pragma region Combat
	/// <summary>
	/// Make player attack an enemy.
	/// </summary>
	/// <param name="Enemy">Enemy to attack.</param>
	void CallBasicAttack(class ABaseEnemy* Enemy);
	#pragma endregion


	#pragma region Cursor Change
	/// <summary>
	/// Update mouse display, according to bIsUsingGamepad variable.
	/// </summary>
	void UpdateMouseDisplay();

	/// <summary>
	/// Check if there was any mouse movement.
	/// </summary>
	void CheckMouseMovement();
	#pragma endregion
	#pragma endregion
#pragma endregion	
};


