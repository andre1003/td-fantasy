// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "BaseCharacter.h"
#include "BaseChest.h"
#include "BaseEnemy.h"
#include "BaseInventory.h"
#include "BaseItem.h"
#include "BasePotion.h"
#include "BaseSkill.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "DamageSystem.h"
#include "Data.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "LevelSystem.h"
#include "Materials/Material.h"
#include "Misc/OutputDeviceNull.h"
#include "UObject/ConstructorHelpers.h"
#pragma endregion


#pragma region Constructor
// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetSphereRadius(150.f);
	InteractionSphere->SetupAttachment(RootComponent);
}
#pragma endregion

#pragma region Events
#pragma region Begin Play
// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Reset stats when begin play
	ResetStats();

	// Setup attachment rules
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::FAttachmentTransformRules
	(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);

	// Setup spawn parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Get inventory and attach it to this character
	if(InventoryClass)
	{
		Inventory = GetWorld()->SpawnActor<ABaseInventory>(InventoryClass, FTransform(), SpawnParams);
		Inventory->AttachToActor(this, AttachmentRules);
		Inventory->SetActorRelativeLocation(FVector::ZeroVector);
	}
	
	if(LevelClass)
	{
		// Get level system and attach it to character
		Level = GetWorld()->SpawnActor<ALevelSystem>(LevelClass, FTransform(), SpawnParams);
		Level->AttachToActor(this, AttachmentRules);
		Level->SetActorRelativeLocation(FVector::ZeroVector);
	}
	
	if(DamageSystemClass)
	{
		// Get damage system and attach it to character
		DamageSystem = GetWorld()->SpawnActor<ADamageSystem>(DamageSystemClass, FTransform(), SpawnParams);
		DamageSystem->AttachToActor(this, AttachmentRules);
		DamageSystem->SetActorRelativeLocation(FVector::ZeroVector);

		// Setup damage system character reference
		DamageSystem->SetCharacter(this);
	}
}
#pragma endregion

#pragma region Tick
// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update all cooldowns
	UpdateCooldowns(DeltaTime);
}
#pragma endregion
#pragma endregion

#pragma region Stats
void ABaseCharacter::ResetStats()
{
	// Health and mana
	Health = BaseHealth;
	Mana = BaseMana;

	// Basic attack
	AttackSpeed = BaseAttackSpeed;
	BasicAttackCooldown = BaseBasicAttackCooldown;
}

void ABaseCharacter::IncreaseHealth(int HealthToAdd)
{
	// Add health points
	Health += HealthToAdd;

	// If exceeds base health, set it to base health
	if(Health > BaseHealth)
	{
		Health = BaseHealth;
	}
		
}

void ABaseCharacter::DecreaseHealth(int HealthToRemove)
{
	// Remove health points
	Health -= HealthToRemove;

	// If health points is less or equal to 0, call die function
	if(Health <= 0)
	{
		Die();
	}
}

void ABaseCharacter::RestoreHealthAndMana(int HealthRestore, int ManaRestore)
{
	// Add health and mana points
	Health += HealthRestore;
	Mana += ManaRestore;

	// If health exceeds base health, set it to base health
	if(Health > BaseHealth)
		Health = BaseHealth;

	// If mana exceeds base mana, set it to base mana
	if(Mana > BaseMana)
		Mana = BaseMana;
}

void ABaseCharacter::PassiveUpgrade(int HealtUpgrade, int ManaUpgrade, int ADUpgrade, int APUpgrade, int TDUpgrade, float BasicAttackCDUpgrade, float AttackSpeedUpgrade)
{
	// Stats upgrade
	BaseHealth = HealtUpgrade;
	BaseMana = ManaUpgrade;

	// Damage upgrade
	DamageSystem->AttackDamage = ADUpgrade;
	DamageSystem->AbilityPower = APUpgrade;
	DamageSystem->TrueDamage = TDUpgrade;

	// Basic attack upgrade
	BaseBasicAttackCooldown = BasicAttackCDUpgrade;
	BaseAttackSpeed = AttackSpeedUpgrade;

	// Reset stats to apply changes
	ResetStats();
}
#pragma endregion

#pragma region Animations
void ABaseCharacter::PlayAttackAnimation()
{
	if(AttackAnimation)
	{
		PlayAnimMontage(AttackAnimation, AttackSpeed);
	}
}
#pragma endregion

#pragma region Combat
bool ABaseCharacter::HasRange(float Range, ESkillType Type)
{
	return false;
}

void ABaseCharacter::UseSkill(int Index)
{
}
#pragma endregion

#pragma region Damage
void ABaseCharacter::BuffDamage(int AttackDamageBuff, int AbilityPowerBuff, int TrueDamageBuff)
{
	// Attack damage buff
	DamageSystem->AttackDamage += AttackDamageBuff;

	// Ability power buff
	DamageSystem->AbilityPower += AbilityPowerBuff;

	// True damage buff
	DamageSystem->TrueDamage += TrueDamageBuff;
}

void ABaseCharacter::DebuffDamage(int AttackDamageDebuff, int AbilityPowerDebuff, int TrueDamageDebuff)
{
	// Attack damage buff
	DamageSystem->AttackDamage -= AttackDamageDebuff;

	// Ability power buff
	DamageSystem->AbilityPower -= AbilityPowerDebuff;

	// True damage buff
	DamageSystem->TrueDamage -= TrueDamageDebuff;
}

void ABaseCharacter::TakeHit(ADamageSystem* EnemyDamageSystem, int PoisonDamage, int FireDamage, int LightningDamage, int BleedingDamage)
{
	// Decrease health points
	Health -= DamageSystem->CalculateTakenDamage(EnemyDamageSystem);

	// Add status
	DamageSystem->AddStatus(PoisonDamage, FireDamage, LightningDamage, BleedingDamage);

	// If health is less or equal to 0, call Die method
	if(Health <= 0)
	{
		Die();
	}
}

void ABaseCharacter::Die()
{
}
#pragma endregion

#pragma region Update
void ABaseCharacter::UpdateCooldowns(float DeltaSeconds)
{
	if(SkillCooldowns.Num() == 0)
	{
		return;
	}

	// Loop through skill cooldowns
	for(int i = 0; i < 4; i++)
	{
		// If cooldown is bigger than 0, decrease DeltaSeconds from it
		if(SkillCooldowns[i] > 0)
		{
			SkillCooldowns[i] -= DeltaSeconds;
		}
	}
}
#pragma endregion

#pragma region Spawn
FTransform ABaseCharacter::GetSkillSpawnTransform(ESkillSpawnOption SpawnOption)
{
	return FTransform();
}

void ABaseCharacter::AttachSkill(ESkillSpawnOption SpawnOption)
{
}

void ABaseCharacter::SpawnHit()
{
}
#pragma endregion
