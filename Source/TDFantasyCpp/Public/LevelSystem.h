// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.

#pragma once

#pragma region Includes
#include "CoreMinimal.h"
#include "Data.h"
#include "GameFramework/Actor.h"
#include "LevelSystem.generated.h"
#pragma endregion


UCLASS()
class TDFANTASYCPP_API ALevelSystem : public AActor
{
	GENERATED_BODY()

#pragma region Attributes
	#pragma region Public Attributes
public:
	#pragma region Level
	// Current level
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Level")
		int Level;

	// Maximum level
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Level")
		int MaxLevel;

	// Available points to upgrade attributes
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Level")
		int AvailablePoints;
	#pragma endregion

	#pragma region Experience
	// Current experience
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Experience")
		int XP;

	// Next level experience
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Experience")
		int NextLevelXP;

	// Base next level experience
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Experience")
		int BaseNextLevelXP;
	#pragma endregion

	#pragma region Attributes
	// Life attribute
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
		int Life;

	// Mana attribute
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
		int Mana;

	// Endurance attribute
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
		int Endurance;

	// Strength attribute
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
		int Strength;

	// Dexterity attribute
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
		int Dexterity;

	// Inteligence attribute
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
		int Inteligence;

	// Arcane attribute
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
		int Arcane;
	#pragma endregion

	#pragma region Upgrade
	// Stats upgrade value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
		int StatsUpgrade;

	// Damage upgrade value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
		int DamageUpgrade;

	// Cooldown upgrade value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
		float CooldownUpgrade;

	// Attack speed upgrade value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
		float AttackSpeedUpgrade;
	#pragma endregion
	#pragma endregion

	#pragma region Protected Attributes
protected:
	// Next level experience multiplier
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Experience")
		int NextLevelMultiplier = 5;

	// Level up UI widget reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UUserWidget> LevelUpWidgetReference;

	// Add XP UI widget reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UUserWidget> AddXPWidgetReference;
	#pragma endregion

	#pragma region Private Attributes

	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	// Sets default values for this actor's properties
	ALevelSystem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	#pragma region Experience
	/// <summary>
	/// Add experience.
	/// </summary>
	/// <param name="AddedXP">- Experience to add.</param>
	void AddXP(int XPToAdd);
	#pragma endregion
	#pragma endregion

	#pragma region Protected Methods
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	#pragma region UI
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Update HUD.
		/// </summary>
		void UpdateHUD();
	#pragma endregion
	#pragma endregion

	#pragma region Private Methods
private:
	#pragma region Level
	/// <summary>
	/// Passive upgrade stats when level up.
	/// </summary>
	void PassiveUpgrade();

	/// <summary>
	/// Level up method.
	/// </summary>
	void LevelUp();
	#pragma endregion

	#pragma region Experience
	/// <summary>
	/// Calculate the experience needed to level up.
	/// </summary>
	void CalculateNextLevelXP();
	#pragma endregion

	#pragma region Attributes
	/// <summary>
	/// Level up a given attribute.
	/// </summary>
	/// <param name="PointsToUpgrade">- Points to upgrade.</param>
	/// <param name="Attribute">- Attribute to upgrade.</param>
	void LevelUpAttribute(int PointsToUpgrade, TEnumAsByte<EPlayerAttributes> Attribute);
	#pragma endregion

	#pragma region Upgrade
	/// <summary>
	/// Increase all upgrade values.
	/// </summary>
	void IncreaseUpgradeValues();
	#pragma endregion

	#pragma region UI
	/// <summary>
	/// Display level up UI.
	/// </summary>
	void DisplayLevelUpUI();

	/// <summary>
	/// Display added XP UI.
	/// </summary>
	void DisplayAddedXPUI(int AddedXP);
	#pragma endregion
	#pragma endregion
#pragma endregion
};
