// Copyright Epic Games, Inc. All Rights Reserved.
// Edit by Paulo André "Dufwine" Pimenta Aragão.


#pragma region Includes
#include "BaseCharacter.h"
#include "BaseBasicHit.h"
#include "BaseChest.h"
#include "BaseEnemy.h"
#include "BaseInventory.h"
#include "BaseItem.h"
#include "BasePlayerController.h"
#include "BasePotion.h"
#include "BaseSkill.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
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
#include "TDFantasyCppGameMode.h"
#include "UObject/ConstructorHelpers.h"
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
	CameraBoom->TargetArmLength = 2200.f;
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
	SkillCooldowns.Add(0.f);	// Skill 1
	SkillCooldowns.Add(0.f);	// Skill 2
	SkillCooldowns.Add(0.f);	// Skill 3
	SkillCooldowns.Add(0.f);	// Ultimate skill

	// Reset stats when begin play
	ResetStats();

	// Setup attachment rules
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::FAttachmentTransformRules
	(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);

	// Spawn potions and add to UsablePotions array
	for (int i = 0; i < Potions.Num(); i++)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABasePotion* NewPotion = GetWorld()->SpawnActor<ABasePotion>(Potions[i], FTransform(), SpawnParams);
		NewPotion->AttachToActor(this, AttachmentRules);
		NewPotion->SetActorRelativeLocation(FVector::ZeroVector);
		UsablePotions.Add(NewPotion);
	}

	// Get inventory and attach it to player
	Inventory = Cast<ABaseInventory>(UGameplayStatics::GetActorOfClass(GetWorld(), InventoryClass));
	Inventory->AttachToActor(this, AttachmentRules);
	Inventory->SetActorRelativeLocation(FVector::ZeroVector);

	// Get level system and attach it to player
	Level = Cast<ALevelSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), LevelClass));
	Level->AttachToActor(this, AttachmentRules);
	Level->SetActorRelativeLocation(FVector::ZeroVector);
}
#pragma endregion

#pragma region Tick
void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Update all cooldowns
	UpdateCooldowns(DeltaSeconds);
}
#pragma endregion

#pragma region Player Input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	#pragma region Axis
	// Gamepad movement axis bind
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::ForwardMovement);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::RightMovement);

	// Zoom axis bind
	PlayerInputComponent->BindAxis("Zoom", this, &ABaseCharacter::CameraZoom);
	#pragma endregion

	#pragma region Actions
	// Gamepad basic hit action bind
	PlayerInputComponent->BindAction("BasicHit", EInputEvent::IE_Pressed, this, &ABaseCharacter::GamepadBasicHit);

	// Skills action bind
	DECLARE_DELEGATE_OneParam(FUseDelegate, const int);
	PlayerInputComponent->BindAction<FUseDelegate>("Skill1", EInputEvent::IE_Pressed, this, &ABaseCharacter::UseSkill, 0);
	PlayerInputComponent->BindAction<FUseDelegate>("Skill2", EInputEvent::IE_Pressed, this, &ABaseCharacter::UseSkill, 1);
	PlayerInputComponent->BindAction<FUseDelegate>("Skill3", EInputEvent::IE_Pressed, this, &ABaseCharacter::UseSkill, 2);
	PlayerInputComponent->BindAction<FUseDelegate>("UltimateSkill", EInputEvent::IE_Pressed, this, &ABaseCharacter::UseSkill, 3);

	// Use potion action bind
	PlayerInputComponent->BindAction<FUseDelegate>("Potion1", EInputEvent::IE_Pressed, this, &ABaseCharacter::UsePotionAtIndex, 0);
	PlayerInputComponent->BindAction<FUseDelegate>("Potion2", EInputEvent::IE_Pressed, this, &ABaseCharacter::UsePotionAtIndex, 1);

	// Reset camera action bind
	PlayerInputComponent->BindAction("ResetCamera", EInputEvent::IE_Pressed, this, &ABaseCharacter::ResetCamera);

	// Use action bind
	PlayerInputComponent->BindAction("Use", EInputEvent::IE_Pressed, this, &ABaseCharacter::Use);

	// Open and close inventory UI action bind
	PlayerInputComponent->BindAction("Inventory", EInputEvent::IE_Pressed, this, &ABaseCharacter::OpenCloseInventoryUI);
	#pragma endregion
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

#pragma region Animations
void ABaseCharacter::PlayAttackAnimation()
{
	if(AttackAnimation)
	{
		PlayAnimMontage(AttackAnimation, AttackSpeed);
	}
}
#pragma endregion

#pragma region Enemy
ABaseEnemy* ABaseCharacter::GetFocusEnemy()
{
	return FocusEnemy;
}
#pragma endregion

#pragma region Combat
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
	ABasePlayerController* PlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->DisableInput(PlayerController);

	// Reset variables
	FocusEnemy = nullptr;
	BasicAttackCooldown = 0.f;
	TempChest = nullptr;
	TempItem = nullptr;

	// Set bIsDead to true
	bIsDead = true;

	// Create game over UI and display it
	UUserWidget* GameOverUI = CreateWidget<UUserWidget>(GetWorld(), GameOverUIClass);
	GameOverUI->AddToViewport();
}
#pragma endregion

#pragma region Attack
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
	// If player is using a skill shot or basic attack is on cooldown, exit
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
	// If is a valid index, skill is on cooldown or there are any temporary variable set, exit method
	if(!Skills.IsValidIndex(Index) || SkillCooldowns[Index] > 0.f || HasTemporary())
	{
		return;
	}

	// Set attack type to skill
	AttackType = EAttackType::EAT_Skill;
	#pragma endregion

	#pragma region Spawn Skill
	// Spawn actor
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentSkill = GetWorld()->SpawnActor<ABaseSkill>(Skills[Index], FTransform(), SpawnParams);
	
	// If there is no range, and spawn option is not OnEnemy nor OnUser, exit
	if(!HasRange(CurrentSkill->Range, CurrentSkill->SkillType) &&
		CurrentSkill->SpawnOption != ESkillSpawnOption::ESSO_OnEnemy &&
		CurrentSkill->SpawnOption != ESkillSpawnOption::ESSO_OnUser)
	{
		CurrentSkill->Destroy();
		CurrentSkill = nullptr;
		return;
	}

	// Setup skill transform
	CurrentSkill->SetActorTransform(GetSkillSpawnTransform(CurrentSkill->SpawnOption));

	// Skill actor setup
	CurrentSkill->DisableSkillCollision();
	AttachSkill(CurrentSkill->SpawnOption);

	// Get skill cost and cooldown
	int HealthCost = CurrentSkill->HealthCost;
	int ManaCost = CurrentSkill->ManaCost;
	float SkillCooldown = CurrentSkill->Cooldown;
	#pragma endregion

	#pragma region Shot Skill
	// If skill type is shot
	if(CurrentSkill->SkillType == ESkillType::EST_Shot)
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
#pragma endregion

#pragma region Spawn
FTransform ABaseCharacter::GetSkillSpawnTransform(ESkillSpawnOption SpawnOption)
{
	// Spawn transform variables
	FVector SpawnLocation = FVector::ZeroVector;
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

	// Create spawn transform
	FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation, SpawnScale);

	// Return spawn transform
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

	if(SpawnOption != ESkillSpawnOption::ESSO_OnPointer)
	{
		// Reset relative position
		CurrentSkill->SetActorRelativeLocation(FVector::Zero());
	}
}

void ABaseCharacter::SpawnHit()
{
	// If attack type is basic attack
	if(AttackType == EAttackType::EAT_BasicAttack)
	{
		// Spawn base basic hit
		FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, GetMesh()->GetSocketLocation(TEXT("RightHandSocket")));
		GetWorld()->SpawnActor<ABaseBasicHit>(BasicHitReference, SpawnTransform);
	}

	// If attack type is skill
	else
	{
		// Start skill movement
		CurrentSkill->StartMovement();

		// Enable skill collision
		CurrentSkill->EnableSkillCollision();

		// Set bIsUsingShotSkill to false
		bIsUsingShotSkill = false;
	}
}
#pragma endregion

#pragma region Wallet
void ABaseCharacter::AddMoney(int CoinsToAdd, int GemsToAdd)
{
	// Add wallet UI
	SetWalletUI(CoinsToAdd, GemsToAdd);
	
	// Add coins and gems
	Coins += CoinsToAdd;
	Gems += GemsToAdd;

	// If coins exceed max allowed money, set it to max money
	if(Coins > MaxMoney)
	{
		Coins = MaxMoney;
	}

	// If gems exceed max allowed money, set it to max money
	if(Gems > MaxMoney)
	{
		Gems = MaxMoney;
	}
}

void ABaseCharacter::SetWalletUI(int AddedCoins, int AddedGems)
{
	// Create wallet UI
	UUserWidget* WalletUI = CreateWidget<UUserWidget>(GetWorld(), WalletUIClass);

	// Set money text
	FOutputDeviceNull OutputDeviceNull;
	const FString Command = FString::Printf(TEXT("SetMoney \"%d\" \"%d\""), AddedCoins, AddedGems);
	WalletUI->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);

	// Add the widget to viewport
	WalletUI->AddToViewport();
}
#pragma endregion

#pragma region Temporary
bool ABaseCharacter::HasTemporary()
{
	return (TempItem != nullptr || TempChest != nullptr);
}
#pragma endregion

#pragma region Update
void ABaseCharacter::UpdateCooldowns(float DeltaSeconds)
{
	// If there is a focus enemy
	if(FocusEnemy != nullptr)
	{
		// If basic attack is on cooldown
		if(BasicAttackCooldown > 0.f)
		{
			BasicAttackCooldown -= DeltaSeconds;
		}

		// If not on cooldow, and focus enemy is not dead, attack it
		else if(!FocusEnemy->GetIsDead())
		{
			BasicAttack(FocusEnemy);
		}
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

#pragma region Gamepad
void ABaseCharacter::ForwardMovement(float Value)
{
	// Set YIn variable
	YIn = Value;
	
	// Fix foward axis diagonal for gamepad
	float ClampValue = UKismetMathLibrary::MapRangeClamped(UKismetMathLibrary::Abs(XIn), 0.f, 0.6f, 1.f, 1.2f) * YIn;
	YOut = UKismetMathLibrary::FClamp(ClampValue, -1.f, 1.f);

	// Call movement input method
	MovementInput(true);
}

void ABaseCharacter::RightMovement(float Value)
{
	// Set XIn variable
	XIn = Value;
	
	// Fix right axis diagonal for gamepad
	float ClampValue = UKismetMathLibrary::MapRangeClamped(UKismetMathLibrary::Abs(YIn), 0.f, 0.6f, 1.f, 1.2f) * XIn;
	XOut = UKismetMathLibrary::FClamp(ClampValue, -1.f, 1.f);

	// Call movement input method
	MovementInput(false);
}

void ABaseCharacter::MovementInput(bool bIsForwardMovement)
{
	// Forward movement
	if(bIsForwardMovement)
	{
		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(FRotator(0.f, GetControlRotation().Yaw, 0.f));
		AddMovementInput(ForwardVector, YOut);
	}

	// Right movement
	else
	{
		FVector RightVector = UKismetMathLibrary::GetRightVector(FRotator(0.f, GetControlRotation().Yaw, 0.f));
		AddMovementInput(RightVector, XOut);
	}

	// If there is any movement, reset focus enemy and auto attack
	if(YOut != 0.f || XOut != 0.f)
	{
		FocusEnemy = nullptr;
		ResetAutoAttack();
	}
}

void ABaseCharacter::GamepadBasicHit()
{
	// Try to find closest enemy in a given range
	GetClosestEnemy(2500.f);

	// If found an enemy, attack it
	if(bEnemyWasFound)
	{
		BasicAttack(EnemyFound);
	}
}
#pragma endregion

#pragma region Camera
void ABaseCharacter::CameraZoom(float Value)
{
	// If there is no input, exit
	if(Value == 0.f)
	{
		return;
	}

	// Get player controller
	ABasePlayerController* PlayerController = Cast<ABasePlayerController>
		(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// If player is using gamepad
	if(PlayerController->GetIsUsingGamepad())
	{
		Zoom = 25.f;
	}

	// If player is not using gamepad
	else
	{
		Zoom = 100.f;
	}


	// If input is positive
	if(Value > 0.f)
	{
		CameraBoom->TargetArmLength -= Zoom;
	}

	// If input is negative
	else
	{
		CameraBoom->TargetArmLength += Zoom;
	}

	// Force target arm length to be between 1000 and 4000
	CameraBoom->TargetArmLength = UKismetMathLibrary::FClamp(CameraBoom->TargetArmLength, 1000.f, 4000.f);
}

void ABaseCharacter::ResetCamera()
{
	CameraBoom->TargetArmLength = 2200.f;
}
#pragma endregion

#pragma region Setters
void ABaseCharacter::SetFocusEnemy(class ABaseEnemy* NewFocusEnemy)
{
	FocusEnemy = NewFocusEnemy;
}

void ABaseCharacter::SetTempItem(ABaseItem* Value)
{
	TempItem = Value;
}

void ABaseCharacter::SetTempChest(class ABaseChest* Value)
{
	TempChest = Value;
}
#pragma endregion

#pragma region Use
void ABaseCharacter::Use()
{
	// If there is no temporary item and chest, exit
	if(!HasTemporary())
	{
		return;
	}

	// If there is a temporary item
	if(TempItem)
	{
		// Add item to inventory and destroy it from world
		Inventory->AddItem(TempItem->GetClass(), 1);
		TempItem->Destroy();
		TempItem = nullptr;
	}

	// If there is a temporary chest
	else
	{
		// Open chest
		TempChest->OpenChest();
	}
}
#pragma endregion

#pragma region Potions
void ABaseCharacter::UsePotionAtIndex(int Index)
{
	// If index is invalid, exit
	if(!UsablePotions.IsValidIndex(Index))
	{
		return;
	}

	// Use the potion
	UsablePotions[Index]->UsePotion();
}
#pragma endregion

#pragma region Inventory
void ABaseCharacter::OpenCloseInventoryUI()
{
	// If there is an inventory reference, call the OpenInventoryUI method
	if(Inventory)
	{
		Inventory->OpenInventoryUI();
	}
}
#pragma endregion
