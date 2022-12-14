// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "LevelSystem.h"
#include "BasePlayableCharacter.h"
#include "Blueprint/UserWidget.h"
#include "DamageSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"
#pragma endregion


#pragma region Constructor
// Sets default values
ALevelSystem::ALevelSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
#pragma endregion

#pragma region Events
#pragma region Begin Play
// Called when the game starts or when spawned
void ALevelSystem::BeginPlay()
{
	Super::BeginPlay();

}
#pragma endregion

#pragma region Tick
// Called every frame
void ALevelSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#pragma endregion
#pragma endregion

#pragma region Experience
void ALevelSystem::AddXP(int XPToAdd)
{
	// Add experience
	XP += XPToAdd;

	// Display added experience UI
	DisplayAddedXPUI(XPToAdd);

	// If experience is bigger or equal the experience needed to level up, level up
	if(XP >= NextLevelXP)
	{
		LevelUp();
	}
}

void ALevelSystem::CalculateNextLevelXP()
{
	NextLevelXP += BaseNextLevelXP * Level * NextLevelMultiplier;
}
#pragma endregion

#pragma region Level
void ALevelSystem::PassiveUpgrade()
{
	// Get player character
	ABasePlayableCharacter* Player = Cast<ABasePlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Get player stats
	int BaseHealth = Player->BaseHealth;
	int BaseMana = Player->BaseMana;

	// Get player damage
	int BaseAttackDamage = Player->DamageSystem->AttackDamage;
	int BaseAbilityPower = Player->DamageSystem->AbilityPower;
	int BaseTrueDamage = Player->DamageSystem->TrueDamage;

	// Get player basic attack cooldown e attack speed
	float BaseBasicAttackCooldown = Player->BaseBasicAttackCooldown;
	float BaseAttackSpeed = Player->BaseAttackSpeed;


	// Upgrade stats
	BaseHealth += StatsUpgrade;
	BaseMana += StatsUpgrade;

	// Upgrade damage
	BaseAttackDamage += DamageUpgrade;
	BaseAbilityPower += DamageUpgrade;
	BaseTrueDamage += DamageUpgrade / 3;

	// Upgrade basic attack cooldown e attack speed
	BaseBasicAttackCooldown -= CooldownUpgrade;
	BaseAttackSpeed += AttackSpeedUpgrade;


	// Apply upgrade to player
	Player->PassiveUpgrade(BaseHealth, BaseMana,
		BaseAttackDamage, BaseAbilityPower, BaseTrueDamage,
		BaseBasicAttackCooldown, BaseAttackSpeed);
}

void ALevelSystem::LevelUp()
{
	// Subtract NextLevelXP from XP (so the remaining XP stays with player)
	XP -= NextLevelXP;

	// If player is already at maximum level, exit
	if(Level == MaxLevel)
	{
		return;
	}

	// Increase level and available points
	Level++;
	AvailablePoints++;

	// Display level up UI
	DisplayLevelUpUI();

	// Passive stats upgrade
	PassiveUpgrade();

	// Calculate next level XP
	CalculateNextLevelXP();

	// Increase upgrade values
	IncreaseUpgradeValues();

	// Update HUD
	UpdateHUD();
}
#pragma endregion

#pragma region Attribute
void ALevelSystem::LevelUpAttribute(int PointsToUpgrade, TEnumAsByte<EPlayerAttributes> Attribute)
{
	// If available points e less than points to upgrade, exit
	if(AvailablePoints < PointsToUpgrade)
	{
		return;
	}


	// Upgrade Life
	if(Attribute == EPlayerAttributes::EPA_Life)
	{
		Life += PointsToUpgrade;
	}

	// Upgrade Mana
	else if(Attribute == EPlayerAttributes::EPA_Mana)
	{
		Mana += PointsToUpgrade;
	}

	// Upgrade Endurance
	else if(Attribute == EPlayerAttributes::EPA_Endurance)
	{
		Endurance += PointsToUpgrade;
	}

	// Upgrade Strength
	else if(Attribute == EPlayerAttributes::EPA_Strength)
	{
		Strength += PointsToUpgrade;
	}

	// Upgrade Dexterity
	else if(Attribute == EPlayerAttributes::EPA_Dexterity)
	{
		Dexterity += PointsToUpgrade;
	}

	// Upgrade Inteligence
	else if(Attribute == EPlayerAttributes::EPA_Inteligence)
	{
		Inteligence += PointsToUpgrade;
	}

	// Upgrade Arcane
	else
	{
		Arcane += PointsToUpgrade;
	}
}
#pragma endregion

#pragma region Upgrade
void ALevelSystem::IncreaseUpgradeValues()
{
	// If current level is 40
	if(Level == 40)
	{
		StatsUpgrade -= 2;
		DamageUpgrade -= 2;
		CooldownUpgrade -= 0.0001f;
		AttackSpeedUpgrade -= 0.002f;
	}

	// If current level is 60
	else if(Level == 60)
	{
		StatsUpgrade -= 3;
		DamageUpgrade -= 3;
		CooldownUpgrade -= 0.00025f;
		AttackSpeedUpgrade -= 0.003f;
	}
}
#pragma endregion

#pragma region UI
void ALevelSystem::DisplayLevelUpUI()
{
	// Create level up UI and add it to viewport
	UUserWidget* LevelUpUI = CreateWidget<UUserWidget>(GetWorld(), LevelUpWidgetReference);
	LevelUpUI->AddToViewport();
}

void ALevelSystem::DisplayAddedXPUI(int AddedXP)
{
	// Create the add XP UI
	UUserWidget* AddXPUI = CreateWidget<UUserWidget>(GetWorld(), AddXPWidgetReference);

	// Set XP text
	FOutputDeviceNull OutputDeviceNull;
	const FString Command = FString::Printf(TEXT("SetXPText \"%d\""), AddedXP);
	AddXPUI->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);

	// Add UI to viewport
	AddXPUI->AddToViewport();
}
#pragma endregion
