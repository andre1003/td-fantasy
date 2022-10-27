// Copyright Epic Games, Inc. All Rights Reserved.
// Edit by Paulo André "Dufwine" Pimenta Aragão.

#pragma region Includes
#include "BaseCharacter.h"
#include "BaseBasicHit.h"
#include "BaseEnemy.h"
#include "BaseSkill.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Data.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "BasePlayerController.h"
#pragma endregion


#pragma region Constructor
ABaseCharacter::ABaseCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}
#pragma endregion

#pragma region Events
#pragma region Begin Play
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set skill cooldowns
	SkillCooldowns[0] = 0.f;
	SkillCooldowns[1] = 0.f;
	SkillCooldowns[2] = 0.f;
	SkillCooldowns[3] = 0.f;

	// Reset stats when begin play
	ResetStats();
}
#pragma endregion

#pragma region Tick
void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
#pragma endregion
#pragma endregion

#pragma region Stats
void ABaseCharacter::ResetStats()
{
	// Health and mana
	Health = BaseHealth;
	Mana = BaseMana;

	// Damage
	AttackDamage = BaseAttackDamage;
	AbilityPower = BaseAbilityPower;
	TrueDamage = BaseTrueDamage;

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
		Health = BaseHealth;
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
	BaseAttackDamage = ADUpgrade;
	BaseAbilityPower = APUpgrade;
	BaseTrueDamage = TDUpgrade;

	// Basic attack upgrade
	BaseBasicAttackCooldown = BasicAttackCDUpgrade;
	BaseAttackSpeed = AttackSpeedUpgrade;

	// Reset stats to apply changes
	ResetStats();
}
#pragma endregion

#pragma region Damage
void ABaseCharacter::BuffDamage(int AttackDamageBuff, int AbilityPowerBuff, int TrueDamageBuff)
{
	// Attack damage buff
	AttackDamage += AttackDamageBuff;

	// Ability power buff
	AbilityPower += AbilityPowerBuff;

	// True damage buff
	TrueDamage += TrueDamageBuff;
}

void ABaseCharacter::DebuffDamage(int AttackDamageDebuff, int AbilityPowerDebuff, int TrueDamageDebuff)
{
	// Attack damage buff
	AttackDamage -= AttackDamageDebuff;

	// Ability power buff
	AbilityPower -= AbilityPowerDebuff;

	// True damage buff
	TrueDamage -= TrueDamageDebuff;
}

void ABaseCharacter::TakeHit(int Damage)
{
	// Decrease health points
	Health -= Damage;

	// If health is less or equal to 0, call Die method
	if(Health <= 0)
		Die();
}

void ABaseCharacter::Die()
{
	// Stop movement immeditely
	GetCharacterMovement()->StopMovementImmediately();

	// Disable input
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// Reset variables
	//
	BasicAttackCooldown = 0.f;
	//
	//

	// Set bIsDead to true
	bIsDead = true;

	// Add game over UI
	//
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
	ABasePlayerController* PlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	// Get using gamepad value
	bool bIsUsingGamepad = PlayerController->GetIsUsingGamepad();
	
	if(bIsUsingGamepad)
	{
		// Set skill target location based on skill type
		if(Type == ESkillType::EST_Shot)
		{
			SkillTarget = GetActorLocation() + (GetActorForwardVector() * Range);
		}
		else
		{
			SkillTarget = ClosestEnemyTargetLocation;
		}

		return true;
	}
	else
	{
		// Get hit under cursor
		FHitResult Hit;
		PlayerController->GetHitResultUnderCursor(ECC_Camera, true, Hit);

		// Calculate distance between player and hit location
		float Distance = UKismetMathLibrary::Vector_Distance(GetActorLocation(), Hit.Location);

		// Set skill target location to hit location
		SkillTarget = Hit.Location;

		// Return distance <= range (if it is, return true, if it's not, return false)
		return (Distance <= Range);
	}
}

void ABaseCharacter::ResetAutoAttack()
{
	BasicAttackCooldown = 0.f;
}

void ABaseCharacter::BasicAttack(ABaseEnemy* EnemyReference)
{
	// If player is using a skill shot or basic attack is in cooldown, exit
	if(bIsUsingShotSkill || BasicAttackCooldown > 0.f)
	{
		return;
	}
	
	// Set focus enemy
	FocusEnemy = EnemyReference;

	// Set actor rotation
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), EnemyReference->GetActorLocation()));

	// Set attack type to basic attack
	AttackType = EAttackType::EAT_BasicAttack;

	// Call play attack animation method
	PlayAttackAnimation();

	// Set basic attack cooldown
	BasicAttackCooldown = BaseBasicAttackCooldown;
}

void ABaseCharacter::UseSkill(int Index)
{
	#pragma region Initial Setup
	// If is a valid index, skill is in cooldown or there are any temporary variable set, exit method
	if(Index > 3 || Index < 0 || SkillCooldowns[Index] > 0.f /* or CheckTemp*/)
	{
		return;
	}

	// Set attack type to skill
	AttackType = EAttackType::EAT_Skill;

	// Get deafult skill attributes
	ABaseSkill* DefaultSkill = Cast<ABaseSkill>(Skills[Index]->StaticClass()->GetDefaultObject(true));
	float SkillRange = DefaultSkill->Range;
	ESkillType SkillType = DefaultSkill->SkillType;
	ESkillSpawnOption SkillSpawnOption = DefaultSkill->SpawnOption;

	// If there is no range, or spawn option is not OnEnemy nor OnUser, exit
	if(!HasRange(SkillRange, SkillType) || (SkillSpawnOption != ESkillSpawnOption::ESSO_OnEnemy && 
		SkillSpawnOption != ESkillSpawnOption::ESSO_OnUser))
	{
		return;
	}
	#pragma endregion

	#pragma region Spawn Skill
	// Spawn actor
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentSkill = GetWorld()->SpawnActor<ABaseSkill>(Skills[Index], GetSkillSpawnTransform(SkillSpawnOption), SpawnParams);
	
	// Skill actor setup
	CurrentSkill->DisableCollision();
	AttachSkill(SkillSpawnOption);

	// Get skill cost and cooldown
	int HealthCost = CurrentSkill->HealthCost;
	int ManaCost = CurrentSkill->ManaCost;
	float SkillCooldown = CurrentSkill->Cooldown;
	#pragma endregion

	#pragma region Shot Skill
	// If skill type is shot
	if(SkillType == ESkillType::EST_Shot)
	{
		// Indicate that is not using shot skill
		bIsUsingShotSkill = true;

		// If player have enought health and mana to use skill
		if(Health >= HealthCost && Mana >= ManaCost)
		{
			// Pay skill cost
			Health -= HealthCost;
			Mana -= ManaCost;

			// Play attack animation
			PlayAttackAnimation();

			// Stop movement immediatly
			GetCharacterMovement()->StopMovementImmediately();

			// Set player rotation
			SetActorRotation(FRotator(0.f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), SkillTarget).Yaw, 0.f));

			SkillCooldowns[Index] = SkillCooldown;
		}
		else
		{
			CurrentSkill->Destroy();
			CurrentSkill = nullptr;
			bIsUsingShotSkill = false;
		}
	}
	#pragma endregion

	#pragma region Conjuration Skill
	// If skill type is conjuration
	else
	{
		// If player have enought health and mana to use skill
		if(Health >= HealthCost && Mana >= ManaCost)
		{
			// Pay skill cost
			Health -= HealthCost;
			Mana -= ManaCost;

			// Set player rotation
			SetActorRotation(FRotator(0.f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), SkillTarget).Yaw, 0.f));

			SkillCooldowns[Index] = SkillCooldown;
		}
		else
		{
			CurrentSkill->Destroy();
			CurrentSkill = nullptr;
		}
	}
	#pragma endregion
}
#pragma endregion

#pragma region Spawn
FTransform ABaseCharacter::GetSkillSpawnTransform(ESkillSpawnOption SpawnOption)
{
	// Spawn transform variables
	FVector SpawnLocation;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FVector SpawnScale = FVector::One();

	// If spawn option is right hand
	if(SpawnOption == ESkillSpawnOption::ESSO_RightHand)
	{
		SpawnLocation = GetMesh()->GetSocketLocation(TEXT("RightHandSocket"));
		SpawnRotation = GetActorRotation();
	}

	// If spawn option is left hand
	else if(SpawnOption == ESkillSpawnOption::ESSO_LeftHand)
	{
		SpawnLocation = GetMesh()->GetSocketLocation(TEXT("LeftHandSocket"));
		SpawnRotation = GetActorRotation();
	}
	
	// If spawn option is above enemy
	else if(SpawnOption == ESkillSpawnOption::ESSO_AboveEnemy)
	{
		SpawnLocation = FocusEnemy->GetActorLocation() + FVector(0.f, 0.f, 200.f);
	}
	
	// If spawn option is on enemy
	else if(SpawnOption == ESkillSpawnOption::ESSO_OnEnemy)
	{
		SpawnLocation = FocusEnemy->GetActorLocation();
	}
	
	// If spawn option is on user
	else if(SpawnOption == ESkillSpawnOption::ESSO_OnUser)
	{
		SpawnLocation = GetActorLocation();
	}
	
	// If spawn option is on pointer
	else
	{
		SpawnLocation = SkillTarget;
	}

	FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation, SpawnScale);

	return SpawnTransform;
}

void ABaseCharacter::AttachSkill(ESkillSpawnOption SpawnOption)
{
	// Attachment rules setup
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::FAttachmentTransformRules
	(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);

	// If spawn option is right hand
	if(SpawnOption == ESkillSpawnOption::ESSO_RightHand)
	{
		CurrentSkill->AttachToComponent(GetMesh(), AttachmentRules, TEXT("RightHandSocket"));
	}

	// If spawn option is left hand
	else if(SpawnOption == ESkillSpawnOption::ESSO_LeftHand)
	{
		CurrentSkill->AttachToComponent(GetMesh(), AttachmentRules, TEXT("LeftHandSocket"));
	}

	// If spawn option is on enemy
	else if(SpawnOption == ESkillSpawnOption::ESSO_OnEnemy)
	{
		CurrentSkill->AttachToComponent(FocusEnemy->GetRootComponent(), AttachmentRules);
	}

	// If spawn option is on user
	else if(SpawnOption == ESkillSpawnOption::ESSO_OnUser)
	{
		CurrentSkill->AttachToComponent(GetRootComponent(), AttachmentRules);
	}

	// Reset relative position
	CurrentSkill->SetActorRelativeLocation(FVector::Zero());
}

void ABaseCharacter::SpawnHit()
{
	// If attack type is basic attack
	if(AttackType == EAttackType::EAT_BasicAttack)
	{
		// Spawn base basic hit
		FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, GetMesh()->GetSocketLocation(TEXT("RightHandSocket")));
		GetWorld()->SpawnActor<ABaseBasicHit>(ABaseBasicHit::StaticClass(), SpawnTransform);
	}

	// If attack type is skill
	else
	{
		// Start skill movement
		CurrentSkill->StartMovement();

		// Enable skill collision
		CurrentSkill->EnableCollision();

		// Set bIsUsingShotSkill to false
		bIsUsingShotSkill = false;
	}
}
#pragma endregion
