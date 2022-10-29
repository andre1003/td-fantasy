// Copyright Epic Games, Inc. All Rights Reserved.
// Edit by Paulo André "Dufwine" Pimenta Aragão.

#pragma once

#pragma region Includes
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data.h"
#include "BaseCharacter.generated.h"
#pragma endregion


UCLASS(Blueprintable)
class ABaseCharacter : public ACharacter
{
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


	// Attack type
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		EAttackType AttackType;
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

	#pragma region Skills
	// Is player using shot skill controller
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skills")
		bool bIsUsingShotSkill;

	// Skill cooldowns vector
	UPROPERTY()
		float SkillCooldowns[4];

	// Skills array
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skills")
		TArray<TSubclassOf<class ABaseSkill>> Skills;

	// Current skill reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skills")
		class ABaseSkill* CurrentSkill;

	// Skill target location
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skills")
		FVector SkillTarget;
	#pragma endregion

	#pragma region Enemy
	// Enemy to focus
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy")
		class ABaseEnemy* FocusEnemy;
	#pragma endregion

	#pragma region Inventory
	// Inventory base class reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
		TSubclassOf<class ABaseInventory> InventoryClass;

	// Player inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		class ABaseInventory* Inventory;
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

	#pragma region Temporary
	// Temporary item reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Temporary")
		class ABaseItem* TempItem;

	// Temporary chest reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Temporary")
		class ABaseChest* TempChest;
	#pragma endregion

	#pragma region Animations
	class UAnimMontage* AttackAnimation;
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
	ABaseCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	// Called when play begins for this actor.
	virtual void BeginPlay() override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }


	#pragma region Stats
	UFUNCTION(BlueprintCallable, Category = "Stats")
		/// <summary>
		/// Increase health points, limiting by the BaseHealth variable.
		/// </summary>
		/// <param name="HealthToAdd">- Health points to be added.</param>
		void IncreaseHealth(int HealthToAdd);

	UFUNCTION(BlueprintCallable, Category = "Stats")
		/// <summary>
		/// Restore health and mana points.
		/// </summary>
		/// <param name="HealthRestore">- Health points to restore.</param>
		/// <param name="ManaRestore">- Mana points to restore.</param>
		void RestoreHealthAndMana(int HealthRestore, int ManaRestore);

	UFUNCTION(BlueprintCallable, Category = "Stats")
		/// <summary>
		/// Passively upgrade player attributes.
		/// </summary>
		/// <param name="HealtUpgrade">- New base health points.</param>
		/// <param name="ManaUpgrade">- New base mana points.</param>
		/// <param name="ADUpgrade">- New base attack damage value.</param>
		/// <param name="APUpgrade">- New base ability power value.</param>
		/// <param name="TDUpgrade">- New base true damage value.</param>
		/// <param name="BasicAttackCDUpgrade">- New base basic attack cooldown value.</param>
		/// <param name="AttackSpeedUpgrade">- New base attack speed value.</param>
		void PassiveUpgrade(int HealtUpgrade, int ManaUpgrade, int ADUpgrade, int APUpgrade, int TDUpgrade, float BasicAttackCDUpgrade, float AttackSpeedUpgrade);
	#pragma endregion

	#pragma region Animations
	UFUNCTION(BlueprintCallable, Category = "Animations")
		/// <summary>
		/// Play the attack animation montage.
		/// </summary>
		void PlayAttackAnimation();
	#pragma endregion

	#pragma region Damage
	UFUNCTION(BlueprintCallable, Category = "Damage")
		/// <summary>
		/// Buff damage stats.
		/// </summary>
		/// <param name="AttackDamageBuff">- Attack damage buff.</param>
		/// <param name="AbilityPowerBuff">- Ability power buff.</param>
		/// <param name="TrueDamageBuff">- True damage buff.</param>
		void BuffDamage(int AttackDamageBuff, int AbilityPowerBuff, int TrueDamageBuff);

	UFUNCTION(BlueprintCallable, Category = "Damage")
		/// <summary>
		/// Debuff damage stats.
		/// </summary>
		/// <param name="AttackDamageDebuff">- Attack damage debuff.</param>
		/// <param name="AbilityPowerDebuff">- Ability power debuff.</param>
		/// <param name="TrueDamageDebuff">- True damage debuff.</param>
		void DebuffDamage(int AttackDamageDebuff, int AbilityPowerDebuff, int TrueDamageDebuff);

	UFUNCTION(BlueprintCallable, Category = "Damage")
		/// <summary>
		/// Take a hit.
		/// </summary>
		/// <param name="Damage">Damage to take.</param>
		void TakeHit(int Damage);

	UFUNCTION(BlueprintCallable, Category = "Damage")
		/// <summary>
		/// Player death.
		/// </summary>
		void Die();
	#pragma endregion

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

	UFUNCTION(BlueprintCallable, Category = "Combat")
		/// <summary>
		/// User a given index skill.
		/// </summary>
		/// <param name="Index">- Skill index.</param>
		void UseSkill(int Index);
	#pragma endregion

	#pragma region Wallet
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Add wallet UI widget to viewport. Blueprint implementable only!
		/// </summary>
		/// <param name="AddedCoins">- Added coins.</param>
		/// <param name="AddedGems">- Added gems.</param>
		void SetWalletUI(int AddedCoins, int AddedGems);

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Add coins and gems to player's wallet.
		/// </summary>
		/// <param name="CoinsToAdd">- Coins to add.</param>
		/// <param name="GemsToAdd">- Gems to add.</param>
		void AddMoney(int CoinsToAdd, int GemsToAdd);
	#pragma endregion

	#pragma region Temporary
	/// <summary>
	/// Set TempItem variable.
	/// </summary>
	/// <param name="Value">- New TempItem value.</param>
	void SetTempItem(class ABaseItem* Value);
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
	/// Set FocusEnemy variable value.
	/// </summary>
	/// <param name="NewFocusEnemy">New FocusEnemy value.</param>
	void SetFocusEnemy(class ABaseEnemy* NewFocusEnemy);
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
	#pragma endregion

	#pragma region Private Methods
private:
	#pragma region Update
	/// <summary>
	/// Update all cooldowns.
	/// </summary>
	/// <param name="DeltaSeconds">- Delta seconds.</param>
	void UpdateCooldowns(float DeltaSeconds);
	#pragma endregion

	#pragma region Stats
	/// <summary>
	/// Reset all stats to base stats.
	/// </summary>
	void ResetStats();
	#pragma endregion

	#pragma region Combat
	/// <summary>
	/// Check if has range for use skill.
	/// </summary>
	/// <param name="Range">- Skill range.</param>
	/// <param name="Type">- Skill type.</param>
	/// <returns>- Has range. TRUE if has range, FALSE if not.</returns>
	bool HasRange(float Range, ESkillType Type);
	#pragma endregion

	#pragma region Spawn
	/// <summary>
	/// Get skill spawn transform based on skill spawn option.
	/// </summary>
	/// <param name="SpawnOption">- Skill spawn option.</param>
	/// <returns>Skill spawn transform.</returns>
	FTransform GetSkillSpawnTransform(ESkillSpawnOption SpawnOption);

	/// <summary>
	/// Attach skill, based on skill spawn option.
	/// </summary>
	/// <param name="SpawnOption">- Skill spawn option.</param>
	void AttachSkill(ESkillSpawnOption SpawnOption);

	/// <summary>
	/// Spawn hit, based on attack type.
	/// </summary>
	void SpawnHit();
	#pragma endregion

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

	#pragma region Use Functionality
	/// <summary>
	/// Use functionality.
	/// </summary>
	void Use();
	#pragma endregion

	#pragma endregion
#pragma endregion
};

