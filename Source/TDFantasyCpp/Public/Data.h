// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.

#pragma once

#pragma region Includes
#include "CoreMinimal.h"
#pragma endregion


#pragma region Skill Type
UENUM(BlueprintType)
enum class ESkillType : uint8
{
	EST_Shot		= 0 UMETA(DisplayName = "Shot"),
	EST_Conjuration = 1 UMETA(DisplayName = "Conjuration"),
};
#pragma endregion

#pragma region Skill Spawn Option
UENUM(BlueprintType)
enum class ESkillSpawnOption : uint8
{
	ESSO_RightHand	= 0	UMETA(DisplayName = "RightHand"),
	ESSO_LeftHand	= 1	UMETA(DisplayName = "LeftHand"),
	ESSO_AboveEnemy = 2	UMETA(DisplayName = "AboveEnemy"),
	ESSO_OnEnemy	= 3	UMETA(DisplayName = "OnEnemy"),
	ESSO_OnUser		= 4	UMETA(DisplayName = "OnUser"),
	ESSO_OnPointer	= 5	UMETA(DisplayName = "OnPointer"),
};
#pragma endregion

#pragma region Attack Type
UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_BasicAttack = 0	UMETA(DisplayName = "BasicAttack"),
	EAT_Skill		= 1	UMETA(DisplayName = "Skill"),
};
#pragma endregion

#pragma region Item Rarity
UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_S = 0 UMETA(DisplayName = "S"),
	EIR_A = 1 UMETA(DisplayName = "A"),
	EIR_B = 2 UMETA(DisplayName = "B"),
	EIR_C = 3 UMETA(DisplayName = "C"),
	EIR_D = 4 UMETA(DisplayName = "D"),
};
#pragma endregion

#pragma region Item Type
UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Equip		= 0	UMETA(DisplayName = "Equip"),
	EIT_Consume		= 0	UMETA(DisplayName = "Consume"),
	EIT_EUpgrade	= 0	UMETA(DisplayName = "Upgrade"),
};
#pragma endregion

#pragma region Player Attributes
UENUM(BlueprintType)
enum class EPlayerAttributes : uint8
{
	EPA_Life		= 0	UMETA(DisplayName = "Life"),
	EPA_Mana		= 0	UMETA(DisplayName = "Mana"),
	EPA_Endurance	= 0	UMETA(DisplayName = "Endurance"),
	EPA_Strength	= 0	UMETA(DisplayName = "Strength"),
	EPA_Dexterity	= 0	UMETA(DisplayName = "Dexterity"),
	EPA_Inteligence = 0	UMETA(DisplayName = "Inteligence"),
	EPA_Arcane		= 0	UMETA(DisplayName = "Arcane"),
};
#pragma endregion

#pragma region Cursor Type
UENUM(BlueprintType)
enum class ECursorType : uint8
{
	ECT_Default		= 0	UMETA(DisplayName = "Default"),
	ECT_Attack		= 1	UMETA(DisplayName = "Attack"),
	ECT_Information	= 2	UMETA(DisplayName = "Information"),
};
#pragma endregion
