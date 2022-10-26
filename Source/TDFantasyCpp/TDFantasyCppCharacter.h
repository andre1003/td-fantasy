// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#pragma region Includes
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TDFantasyCppCharacter.generated.h"
#pragma endregion


UCLASS(Blueprintable)
class ATDFantasyCppCharacter : public ACharacter {
	GENERATED_BODY()


#pragma region Atributes
#pragma region Public Attributes

#pragma endregion

#pragma region Protected Attributes
protected:
	#pragma region Stats
	// Health points
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		int Health;

	// Mana points
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		int Mana;


	// Attack speed value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		float AttackSpeed;

	// Basic attack cooldown value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		float BasicAttackCooldown;

	// Camera zoom value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		float Zoom;


	// Is player dead controller
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		bool bIsDead;
	#pragma endregion

	#pragma region Base Stats
	// Base health points
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Stats")
		int BaseHealth;

	// Base mana points
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Stats")
		int BaseMana;

	// Base attack damage (AD)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Stats")
		int BaseAttackDamage;

	// Base ability power (AP)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Stats")
		int BaseAbilityPower;

	// Base true damage (TD)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Stats")
		int BaseTrueDamage;


	// Base attack speed value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Stats")
		float BaseAttackSpeed;

	// Base basic attack cooldown value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Stats")
		float BaseBasicAttackCooldown;
	#pragma endregion

	#pragma region Damage
	// Attack damage (AD)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int AttackDamage;

	// Ability power (AP)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int AbilityPower;

	// True damage (TD)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int TrueDamage;
	#pragma endregion

	#pragma region Wallet
	// Coins
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wallet")
		int Coins;

	// Gems
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wallet")
		int Gems;

	// Maximum money
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wallet")
		int MaxMoney = 1000000;
	#pragma endregion
#pragma endregion

#pragma region Private Attributes
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
#pragma endregion
#pragma endregion


#pragma region Methods
#pragma region Public Methods
public:
	ATDFantasyCppCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	// Called when play begins for this actor.
	virtual void BeginPlay() override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
#pragma endregion

#pragma region Protected Methods

#pragma endregion

#pragma region Private Methods
private:
	#pragma region Stats
	/// <summary>
	/// Reset all stats to base stats.
	/// </summary>
	void ResetStats();
	#pragma endregion

	
#pragma endregion
#pragma endregion
};

