// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.

#pragma once

#pragma region Includes
#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Data.h"
#include "BaseEnemy.generated.h"
#pragma endregion


UCLASS()
class TDFANTASYCPP_API ABaseEnemy : public ABaseCharacter
{
	GENERATED_BODY()


#pragma region Attributes
	#pragma region Public Attributes
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UWidgetComponent* HealthUI;
	#pragma endregion

	#pragma region Protected Attributes
protected:
	#pragma region Stats
	// Maximum allowed distance from enemy
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		float MaxDistanceFromEnemy;

	// Rage cooldown value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		float RageCooldown;

	// Range for calling friends
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		float CallFriendsRange;


	// Can see enemy (player) controller
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		bool bCanSeeEnemy;

	// Is attacking controller
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		bool bIsAttacking;

	// Is angry controller
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		bool bIsAngry;

	// Has performed right attack controller
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		bool bHasRightAttacked;

	// Can rage controller
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		bool bCanRage;


	// Player reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		ACharacter* PlayerReference;
	#pragma endregion

	#pragma region Damage
	// Temporary damage suffered (for UI control)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int TempDamageSuffered;
	#pragma endregion

	#pragma region Rewards
	// Experience when kill enemy
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rewards")
		int XP;

	// Coins when kill enemy
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rewards")
		int Coins;

	// Gems when kill enemy
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rewards")
		int Gems;


	// Chance of give coins (0.0f = 0% and 1.f = 100%)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rewards")
		float CoinPercent = 0.25f;

	// Chance of give gems (0.0f = 0% and 1.f = 100%)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rewards")
		float GemPercent = 0.001f;
	#pragma endregion

	#pragma region Materials
	// Normal enemy material
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
		UMaterialInstance* NormalMaterial;

	// Rage enemy material
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
		UMaterialInstance* RageMaterial;
	#pragma endregion

	#pragma region Animations
	// Right attack animation
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
		UAnimMontage* AttackRight;

	// Left attack animation
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
		UAnimMontage* AttackLeft;

	// Take hit animation
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
		UAnimMontage* Hit;
	#pragma endregion
	#pragma endregion

	#pragma region Private Attributes
private:
	// Rage timer handle
	FTimerHandle RageTimer;
	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	// Sets default values for this character's properties
	ABaseEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Get bIsDead value.
		/// </summary>
		/// <returns>bIsDead value.</returns>
		bool GetIsDead();

	/// <summary>
	/// Make enemy take a skill hit.
	/// </summary>
	/// <param name="EnemyDamageSystem">- Enemy damage system reference.</param>
	/// <param name="PoisonDamage">- Enemy poison damage.</param>
	/// <param name="FireDamage">- Enemy fire damage.</param>
	/// <param name="LightningDamage">- Enemy lightning damage.</param>
	/// <param name="BleedingDamage">- Enemy bleeding damage.</param>
	virtual void TakeHit(class ADamageSystem* EnemyDamageSystem, int PoisonDamage = 0, int FireDamage = 0, int LightningDamage = 0, int BleedingDamage = 0) override;
	
	/// <summary>
	/// Decrease health points.
	/// </summary>
	/// <param name="HealthToRemove">- Healt points to remove.</param>
	virtual void DecreaseHealth(int HealthToRemove) override;
	#pragma endregion

	#pragma region Protected Methods
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	#pragma region UI
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Setup enemy UI widget.
		/// </summary>
		void SetupUI();
	#pragma endregion

	#pragma region Rage
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Setup rage effect.
		/// </summary>
		void RageEffect();
	#pragma endregion

	#pragma region Tick Event Auxiliar Methods
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Make enemy patrol.
		/// </summary>
		void Patrol();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Make enemy stop chase player.
		/// </summary>
		bool StopChasePlayer();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Make enemy chase player.
		/// </summary>
		void ChasePlayer();
	#pragma endregion

	#pragma region Clear Effect
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Clear a visual effect.
		/// </summary>
		void ClearVisualEffect(UParticleSystemComponent* Effect, float LifeSpan);
	#pragma endregion

	#pragma region Reset Temp Damage
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Reset temporary damage.
		/// </summary>
		void ClearTempDamage();
	#pragma endregion

	#pragma region Combat
	#pragma region Take Damage
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Slow enemy down.
		/// </summary>
		void SlowDown();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Setup damage effect.
		/// </summary>
		void DamageEffect();
	#pragma endregion

	#pragma region Attack Cooldown
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Start counting attack cooldown.
		/// </summary>
		void StartAttackCooldown();
	#pragma endregion

	#pragma region Death Effect
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Apply fade death effect.
		/// </summary>
		void DeathEffect();
	#pragma endregion

	#pragma region Call Friends
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Call friends when take hit.
		/// </summary>
		void CallFriends();
	#pragma endregion
	#pragma endregion
	#pragma endregion

	#pragma region Private Methods
private:

	#pragma region Rage
	/// <summary>
	/// Randomize enemy's rage method.
	/// </summary>
	void RageController();
	#pragma endregion

	#pragma region Enemy Movement
	/// <summary>
	/// Update enemy movement.
	/// </summary>
	void UpdateMovement();
	#pragma endregion

	#pragma region Die
	/// <summary>
	/// Die method.
	/// </summary>
	virtual void Die() override;
	#pragma endregion

	#pragma region Combat
	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Start enemy attack.
		/// </summary>
		void StartAttack();

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Do damage on a given actor.
		/// </summary>
		/// <param name="Actor">- Player actor reference.</param>
		void DoDamage(class AActor* Actor);
	#pragma endregion

	#pragma region Rewards
	/// <summary>
	/// Give rewards to player.
	/// </summary>
	void GiveRewards();
	#pragma endregion
	#pragma endregion
#pragma endregion
};
