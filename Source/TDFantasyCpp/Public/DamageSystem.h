// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.

#pragma once

#pragma region Includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageSystem.generated.h"
#pragma endregion


UCLASS()
class TDFANTASYCPP_API ADamageSystem : public AActor
{
	GENERATED_BODY()


#pragma region Attributes
	#pragma region Public Attributes
public:
	#pragma region Damage
	// Attack damage (AD)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int AttackDamage;

	// Abilty power (AP)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int AbilityPower;

	// True damage (TD)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int TrueDamage;

	// Poison damage (PD)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int PoisonDamage;

	// Fire damage (FD)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int FireDamage;

	// Lightning damage (LD)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int LightningDamage;

	// Bleeding damage (BD)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int BleedingDamage;
	#pragma endregion
	#pragma endregion

	#pragma region Protected Attributes
protected:
	#pragma region Status
	// Poison status
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
		int PoisonStatus;

	// Fire status
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
		int FireStatus;

	// Lightning status
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
		int LightningStatus;

	// Bleeding status
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
		int BleedingStatus;
	#pragma endregion

	#pragma region Maximum Status
	// Maximum poison status
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Maximum Status")
		int MaxPoisonStatus;

	// Maximum fire status
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Maximum Status")
		int MaxFireStatus;

	// Maximum lightning status
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Maximum Status")
		int MaxLightningStatus;

	// Maximum bleeding status
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Maximum Status")
		int MaxBleedingStatus;
	#pragma endregion

	#pragma region Armor
	// Attack damage armor
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Armor")
		int PhysicalArmor;

	// Abilty power armor
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Armor")
		int MagicArmor;

	// Poison damage armor
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Armor")
		int PoisonArmor;

	// Fire damage armor
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Armor")
		int FireArmor;

	// Lightning damage armor
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Armor")
		int LightningArmor;

	// Bleeding damage armor
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Armor")
		int BleedingArmor;
	#pragma endregion
	#pragma endregion

	#pragma region Private Attributes
private:
	#pragma region Timers
	// Poision timer
	FTimerHandle PoisonTimer;

	// Fire timer
	FTimerHandle FireTimer;

	// Lightning timer
	FTimerHandle LightningTimer;

	// Bleeding timer
	FTimerHandle BleedingTimer;
	#pragma endregion
	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	// Sets default values for this actor's properties
	ADamageSystem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	#pragma region Damage
	/// <summary>
	/// Calculate taken damage for this user, based on enemy's damage system.
	/// </summary>
	/// <param name="EnemyDamageSystem">- Enemy's damage system.</param>
	/// <returns></returns>
	int CalculateTakenDamage(class ADamageSystem* EnemyDamageSystem);
	#pragma endregion

	#pragma region Status
	/// <summary>
	/// Add status.
	/// </summary>
	/// <param name="PoisonDamage">- Poison damage to add.</param>
	/// <param name="FireDamage">- Fire damage to add.</param>
	/// <param name="LightningDamage">- Lightning damage to add.</param>
	/// <param name="BleedingDamage">- Bleeding damage to add.</param>
	void AddStatus(int AddedPoisonDamage, int AddedFireDamage, int AddedLightningDamage, int AddedBleedingDamage);
	#pragma endregion
	#pragma endregion

	#pragma region Protected Methods
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	#pragma endregion

	#pragma region Private Methods
private:

	#pragma region Status
	/// <summary>
	/// Poison user.
	/// </summary>
	void Poison();

	/// <summary>
	/// Flame user.
	/// </summary>
	void Fire();

	/// <summary>
	/// Shock the user.
	/// </summary>
	void Lightning();

	/// <summary>
	/// Bleed user.
	/// </summary>
	void Bleeding();

	/// <summary>
	/// Check all status.
	/// </summary>
	void CheckAllStatus();
	#pragma endregion	
	#pragma endregion
#pragma endregion
};
