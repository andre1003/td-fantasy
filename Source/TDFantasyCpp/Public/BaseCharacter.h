// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.

#pragma once

#pragma region Include
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseInventory.h"
#include "BaseSkill.h"
#include "Data.h"
#include "Misc/OutputDeviceNull.h"
#include "BaseCharacter.generated.h"
#pragma endregion


UCLASS()
class TDFANTASYCPP_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()


#pragma region Atributes
	#pragma region Public Attributes
public:
	#pragma region Interaction
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* InteractionSphere;
	#pragma endregion


	#pragma region Damage
	// Damage system 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		class ADamageSystem* DamageSystem;

	// Damage system class reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		TSubclassOf<class ADamageSystem> DamageSystemClass;
	#pragma endregion

	#pragma region Base Stats
	// Base health points
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Stats")
		int BaseHealth;

	// Base mana points
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Stats")
		int BaseMana;


	// Base attack speed value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Stats")
		float BaseAttackSpeed;

	// Base basic attack cooldown value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Stats")
		float BaseBasicAttackCooldown;
	#pragma endregion

	#pragma region Inventory
	// Inventory base class reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
		TSubclassOf<class ABaseInventory> InventoryClass;

	// Player inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		class ABaseInventory* Inventory;
	#pragma endregion

	#pragma region Level
	// Level system class reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Level")
		TSubclassOf<class ALevelSystem> LevelClass;

	// Level system
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
		class ALevelSystem* Level;
	#pragma endregion
	#pragma endregion

	#pragma region Protected Attributes
protected:
	#pragma region Animations
	// Attack animation
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
		class UAnimMontage* AttackAnimation;
	#pragma endregion

	#pragma region Skills
	// Is player using shot skill controller
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skills")
		bool bIsUsingShotSkill;

	// Skill cooldowns vector
	UPROPERTY(BlueprintReadOnly, Category = "Skills")
		TArray<float> SkillCooldowns;

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


	// Is player dead controller
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		bool bIsDead;
	#pragma endregion	
	#pragma endregion

	#pragma region Private Attributes

	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	// Sets default values for this character's properties
	ABaseCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	// Called when play begins for this actor.
	virtual void BeginPlay() override;


	#pragma region Animations
	UFUNCTION(BlueprintCallable, Category = "Animations")
		/// <summary>
		/// Play the attack animation montage.
		/// </summary>
		void PlayAttackAnimation();
	#pragma endregion

	#pragma region Basic Hit
	// Basic hit class reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BasicHit")
		TSubclassOf<class ABaseBasicHit> BasicHitReference;
	#pragma endregion

	#pragma region Combat
	UFUNCTION(BlueprintCallable, Category = "Combat")
		/// <summary>
		/// User a given index skill.
		/// </summary>
		/// <param name="Index">- Skill index.</param>
		virtual void UseSkill(int Index);
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
		/// <param name="EnemyDamageSystem">- Enemy damage system reference.</param>
		/// <param name="PoisonDamage">- Enemy poison damage.</param>
		/// <param name="FireDamage">- Enemy fire damage.</param>
		/// <param name="LightningDamage">- Enemy lightning damage.</param>
		/// <param name="BleedingDamage">- Enemy bleeding damage.</param>
		virtual void TakeHit(class ADamageSystem* EnemyDamageSystem, int PoisonDamage = 0, int FireDamage = 0, int LightningDamage = 0, int BleedingDamage = 0);

	UFUNCTION(BlueprintCallable, Category = "Damage")
		/// <summary>
		/// Base character death.
		/// </summary>
		virtual void Die();
	#pragma endregion

	#pragma region Stats
	UFUNCTION(BlueprintCallable, Category = "Stats")
		/// <summary>
		/// Increase health points, limiting by the BaseHealth variable.
		/// </summary>
		/// <param name="HealthToAdd">- Health points to be added.</param>
		void IncreaseHealth(int HealthToAdd);

	UFUNCTION(BlueprintCallable, Category = "Stats")
		/// <summary>
		/// Decrease health points.
		/// </summary>
		/// <param name="HealthToRemove">- Healt points to remove.</param>
		virtual void DecreaseHealth(int HealthToRemove);

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
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
		/// <summary>
		/// Reset all stats to base stats.
		/// </summary>
		void ResetStats();
	#pragma endregion

	#pragma endregion

	#pragma region Protected Methods
protected:
	#pragma region Spawn
	/// <summary>
	/// Get skill spawn transform based on skill spawn option.
	/// </summary>
	/// <param name="SpawnOption">- Skill spawn option.</param>
	/// <returns>Skill spawn transform.</returns>
	virtual FTransform GetSkillSpawnTransform(ESkillSpawnOption SpawnOption);

	/// <summary>
	/// Attach skill, based on skill spawn option.
	/// </summary>
	/// <param name="SpawnOption">- Skill spawn option.</param>
	virtual void AttachSkill(ESkillSpawnOption SpawnOption);

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Spawn hit, based on attack type.
		/// </summary>
		virtual void SpawnHit();
	#pragma endregion

	#pragma region Update
	/// <summary>
	/// Update all cooldowns.
	/// </summary>
	/// <param name="DeltaSeconds">- Delta seconds.</param>
	virtual void UpdateCooldowns(float DeltaSeconds);
	#pragma endregion
	#pragma endregion

	#pragma region Private Methods
private:
	#pragma region Combat
	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Check if has range for use skill.
		/// </summary>
		/// <param name="Range">- Skill range.</param>
		/// <param name="Type">- Skill type.</param>
		/// <returns>- Has range. TRUE if has range, FALSE if not.</returns>
		virtual bool HasRange(float Range, ESkillType Type);
	#pragma endregion
	#pragma endregion
#pragma endregion
};
