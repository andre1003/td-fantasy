// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.

#pragma once

#pragma region Includes
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"
#pragma endregion


UCLASS()
class TDFANTASYCPP_API ABaseEnemy : public ACharacter {
	GENERATED_BODY()


#pragma region Attributes
	#pragma region Public Attributes

	#pragma endregion

	#pragma region Protected Attributes
protected:
	#pragma region Stats
	// Base health points
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		int BaseHealth;
	
	// Health points
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		int Health;


	// Attack cooldown value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		float AttackCooldown;

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

	// Is dead controller
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		bool bIsDead;

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

	#pragma region Armor
	// AD armor
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Armor")
		int AttackArmor;

	// AP armor
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Armor")
		int MagicArmor;
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

	// Temporary damage suffered (for UI control)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int TempDamageSuffered;
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
	// Rage timer handle
	FTimerHandle RageTimer;
	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	// Sets default values for this character's properties
	ABaseEnemy();


	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Get bIsDead value.
		/// </summary>
		/// <returns>bIsDead value.</returns>
		bool GetIsDead();
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
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	#pragma endregion
#pragma endregion
};
