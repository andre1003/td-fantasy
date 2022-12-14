// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.

#pragma once

#pragma region Includes
#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Data.h"
#include "BasePlayableCharacter.generated.h"
#pragma endregion

/**
 * 
 */
UCLASS()
class TDFANTASYCPP_API ABasePlayableCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
#pragma region Attributes
	#pragma region Public Attributes

	#pragma endregion

	#pragma region Protected Attributes
protected:
	#pragma region Combat
	// Game over UI widget reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
		TSubclassOf<class UUserWidget> GameOverUIClass;
	#pragma endregion

	#pragma region Enemy
	// Enemy to focus
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy")
		class ABaseEnemy* FocusEnemy;
	#pragma endregion

	#pragma region Get Closest Enemy Variables
	// Closest enemy target
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Closest Enemy")
		FVector ClosestEnemyTargetLocation;

	// Any enemy was found controller
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Closest Enemy")
		bool bEnemyWasFound;

	// Enemy found reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Closest Enemy")
		class ABaseEnemy* EnemyFound;
	#pragma endregion

	#pragma region Potions
	// Potions slots (this will only have max 4 slots, but is better use TArray)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Potions")
		TArray<TSubclassOf<class ABasePotion>> Potions;

	// Potions objects
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potions")
		TArray<class ABasePotion*> UsablePotions;
	#pragma endregion

	#pragma region Stats
	// Camera zoom value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		float Zoom;

	// Attack type
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		EAttackType AttackType;
	#pragma endregion

	#pragma region Temporary
	// Temporary item reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Temporary")
		class ABaseItem* TempItem;

	// Temporary chest reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Temporary")
		class ABaseChest* TempChest;
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


	// Wallet UI widget class reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wallet")
		TSubclassOf<class UUserWidget> WalletUIClass;
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

	#pragma region Movement
	// Original Y scale value
	float YIn;

	// Original X scale value
	float XIn;

	// Fixed Y scale value
	float YOut;

	// Fixed X scale value
	float XOut;
	#pragma endregion
	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	// Constructor method.
	ABasePlayableCharacter();

	// Called when play begins for this actor.
	virtual void BeginPlay() override;


	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	#pragma region Combat
	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Reset auto attack.
		/// </summary>
		void ResetAutoAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
		/// <summary>
		/// Perform a basic attack on an enemy.
		/// </summary>
		/// <param name="EnemyReference">- Target enemy reference.</param>
		void BasicAttack(class ABaseEnemy* EnemyReference);

	
	/// <summary>
	/// User a given index skill.
	/// </summary>
	/// <param name="Index">- Skill index.</param>
	virtual void UseSkill(int Index) override;

	/// <summary>
	/// Player death.
	/// </summary>
	virtual void Die() override;
	#pragma endregion

	#pragma region Enemy
	/// <summary>
	/// Get FocusEnemy variable.
	/// </summary>
	/// <return>FocusEnemy value.</return>
	class ABaseEnemy* GetFocusEnemy();

	/// <summary>
	/// Set FocusEnemy variable value.
	/// </summary>
	/// <param name="NewFocusEnemy">New FocusEnemy value.</param>
	void SetFocusEnemy(class ABaseEnemy* NewFocusEnemy);
	#pragma endregion

	#pragma region Movement
	/// <summary>
	/// Perform gamepad movement input.
	/// </summary>
	/// <param name="bIsForwardMovement">Is forward movement controller.</param>
	void MovementInput(bool bIsForwardMovement);
	#pragma endregion

	#pragma region Setters
	/// <summary>
	/// Set TempItem variable.
	/// </summary>
	/// <param name="Value">- New TempItem value.</param>
	void SetTempItem(class ABaseItem* Value);

	/// <summary>
	/// Set TemChest variable
	/// </summary>
	/// <param name="Value">- New TempChest value.</param>
	void SetTempChest(class ABaseChest* Value);

	void SetCoins(int Value);

	void SetGems(int Value);
	#pragma endregion

	#pragma region Getters
	int GetCoins();

	int GetGems();
	#pragma endregion

	#pragma region Wallet
	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Add coins and gems to player's wallet.
		/// </summary>
		/// <param name="CoinsToAdd">- Coins to add.</param>
		/// <param name="GemsToAdd">- Gems to add.</param>
		void AddMoney(int CoinsToAdd, int GemsToAdd);
	#pragma endregion
	#pragma endregion

	#pragma region Protected Methods
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	#pragma region Combat
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Try to get the closest enemy.
		/// </summary>
		void GetClosestEnemy(float Range);
	#pragma endregion

	#pragma region Spawn
	/// <summary>
	/// Get skill spawn transform based on skill spawn option.
	/// </summary>
	/// <param name="SpawnOption">- Skill spawn option.</param>
	/// <returns>Skill spawn transform.</returns>
	virtual FTransform GetSkillSpawnTransform(ESkillSpawnOption SpawnOption) override;

	/// <summary>
	/// Attach skill, based on skill spawn option.
	/// </summary>
	/// <param name="SpawnOption">- Skill spawn option.</param>
	virtual void AttachSkill(ESkillSpawnOption SpawnOption) override;

	/// <summary>
	/// Spawn hit, based on attack type.
	/// </summary>
	virtual void SpawnHit() override;
	#pragma endregion

	#pragma region Update
	/// <summary>
	/// Update all cooldowns.
	/// </summary>
	/// <param name="DeltaSeconds">- Delta seconds.</param>
	virtual void UpdateCooldowns(float DeltaSeconds) override;
	#pragma endregion

	#pragma region Interaction
	/// <summary>
	/// Check if there is any interaction.
	/// </summary>
	/// <returns>TRUE if there are interactions. FALSE if not.</returns>
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		bool HasInteractions();

	/// <summary>
	/// Interact method.
	/// </summary>
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void Interact();
	#pragma endregion
	#pragma endregion

	#pragma region Private Methods
private:
	#pragma region Temporary
	/// <summary>
	/// Chek if there is any temporary variable set (item or chest).
	/// </summary>
	/// <returns>Return if there is a temporary item or chest. TRUE if there is, FALSE if there is not.</returns>
	bool HasTemporary();
	#pragma endregion

	#pragma region Gamepad
	/// <summary>
	/// Forward movement bind method.
	/// </summary>
	/// <param name="Value">Scale value.</param>
	void ForwardMovement(float Value);

	/// <summary>
	/// Right movement bind method.
	/// </summary>
	/// <param name="Value">Scale value.</param>
	void RightMovement(float Value);

	/// <summary>
	/// Perform a basic hit when using gamepad.
	/// </summary>
	void GamepadBasicHit();
	#pragma endregion

	#pragma region Camera
	/// <summary>
	/// Camera zoom in or out.
	/// </summary>
	/// <param name="Value">Axis value.</param>
	void CameraZoom(float Value);

	/// <summary>
	/// Reset camera position to default.
	/// </summary>
	void ResetCamera();
	#pragma endregion

	#pragma region Combat
	/// <summary>
	/// Check if has range for use skill.
	/// </summary>
	/// <param name="Range">- Skill range.</param>
	/// <param name="Type">- Skill type.</param>
	/// <returns>- Has range. TRUE if has range, FALSE if not.</returns>
	virtual bool HasRange(float Range, ESkillType Type) override;
	#pragma endregion

	#pragma region Use Functionality
	/// <summary>
	/// Use functionality.
	/// </summary>
	void Use();
	#pragma endregion

	#pragma region Potions
	/// <summary>
	/// Use the Index potion in Potions array.
	/// </summary>
	/// <param name="Index">Potion to use.</param>
	void UsePotionAtIndex(int Index);
	#pragma endregion

	#pragma region Wallet
	/// <summary>
	/// Add wallet UI widget to viewport. Blueprint implementable only!
	/// </summary>
	/// <param name="AddedCoins">- Added coins.</param>
	/// <param name="AddedGems">- Added gems.</param>
	void SetWalletUI(int AddedCoins, int AddedGems);
	#pragma endregion

	#pragma region Inventory
	/// <summary>
	/// Open or close inventory UI.
	/// </summary>
	void OpenCloseInventoryUI();
	#pragma endregion
	#pragma endregion
#pragma endregion
};
