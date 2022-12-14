// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.

#pragma once

#pragma region Includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data.h"
#include "BaseSkill.generated.h"
#pragma endregion


UCLASS()
class TDFANTASYCPP_API ABaseSkill : public AActor
{
	GENERATED_BODY()
	
#pragma region Attributes
	#pragma region Public Attributes
public:
	#pragma region Damage
	// Attack damage value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int AttackDamage;

	// Ability power value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int AbilityPower;

	// True damage value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		int TrueDamage;
	#pragma endregion

	#pragma region Stats
	// Cooldown value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		float Cooldown;

	// Effect duration value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		float EffectDuration;

	// Skill range
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		float Range;

	// Skill type
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		ESkillType SkillType;

	// Spawn option
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		ESkillSpawnOption SpawnOption;
	#pragma endregion

	#pragma region Cost
	// Health cost value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cost")
		int HealthCost;

	// Mana cost value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cost")
		int ManaCost;
	#pragma endregion

	#pragma region UI Information
	// HUD text
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI Information")
		FText HUDText;
	#pragma endregion
	#pragma endregion

	#pragma region Protected Attributes

	#pragma endregion

	#pragma region Private Attributes

	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	// Sets default values for this actor's properties
	ABaseSkill();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		/// <summary>
		/// Begin overlap override method.
		/// </summary>
		void OverlapBegin(AActor* OverlappedActor, AActor* OtherActor);


	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Start skill movement.
		/// </summary>
		void StartMovement();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Enable collision. Must be override, if needed.
		/// </summary>
		void EnableSkillCollision();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Disable collision. Must be override, if needed.
		/// </summary>
		void DisableSkillCollision();
	#pragma endregion

	#pragma region Protected Methods
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	#pragma endregion

	#pragma region Private Methods

	#pragma endregion
#pragma endregion
};
