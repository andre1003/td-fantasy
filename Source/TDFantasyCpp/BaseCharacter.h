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
#pragma endregion

	#pragma region Protected Methods
protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	/// <summary>
	/// Try to get the closest enemy.
	/// </summary>
	void GetClosestEnemy();
	#pragma endregion

	#pragma region Private Methods
private:
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

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Reset auto attack.
		/// </summary>
		void ResetAutoAttack();
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
	#pragma endregion
#pragma endregion
};

