// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "BasePlayableCharacter.h"
#include "BaseBasicHit.h"
#include "BaseChest.h"
#include "BaseEnemy.h"
#include "BaseItem.h"
#include "BasePotion.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DamageSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "LevelSystem.h"
#include "TDFPlayerController.h"
#pragma endregion


#pragma region Constructor
ABasePlayableCharacter::ABasePlayableCharacter()
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
}
#pragma endregion

#pragma region Events
#pragma region Begin Play
void ABasePlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set skill cooldowns
	SkillCooldowns.Add(0.f);	// Skill 1
	SkillCooldowns.Add(0.f);	// Skill 2
	SkillCooldowns.Add(0.f);	// Skill 3
	SkillCooldowns.Add(0.f);	// Ultimate skill

	// Setup attachment rules
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::FAttachmentTransformRules
	(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);

	// Setup spawn parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn potions and add to UsablePotions array
	for(int i = 0; i < Potions.Num(); i++)
	{
		ABasePotion* NewPotion = GetWorld()->SpawnActor<ABasePotion>(Potions[i], FTransform(), SpawnParams);
		NewPotion->AttachToActor(this, AttachmentRules);
		NewPotion->SetActorRelativeLocation(FVector::ZeroVector);
		UsablePotions.Add(NewPotion);
	}
}
#pragma endregion

#pragma region Player Input
// Called to bind functionality to input
void ABasePlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	#pragma region Axis
	// Gamepad movement axis bind
	PlayerInputComponent->BindAxis("MoveForward", this, &ABasePlayableCharacter::ForwardMovement);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABasePlayableCharacter::RightMovement);

	// Zoom axis bind
	PlayerInputComponent->BindAxis("Zoom", this, &ABasePlayableCharacter::CameraZoom);
	#pragma endregion

	#pragma region Actions
	// Gamepad basic hit action bind
	PlayerInputComponent->BindAction("BasicHit", EInputEvent::IE_Pressed, this, &ABasePlayableCharacter::GamepadBasicHit);

	// Skills action bind
	DECLARE_DELEGATE_OneParam(FUseDelegate, const int);
	PlayerInputComponent->BindAction<FUseDelegate>("Skill1", EInputEvent::IE_Pressed, this, &ABasePlayableCharacter::UseSkill, 0);
	PlayerInputComponent->BindAction<FUseDelegate>("Skill2", EInputEvent::IE_Pressed, this, &ABasePlayableCharacter::UseSkill, 1);
	PlayerInputComponent->BindAction<FUseDelegate>("Skill3", EInputEvent::IE_Pressed, this, &ABasePlayableCharacter::UseSkill, 2);
	PlayerInputComponent->BindAction<FUseDelegate>("UltimateSkill", EInputEvent::IE_Pressed, this, &ABasePlayableCharacter::UseSkill, 3);

	// Use potion action bind
	PlayerInputComponent->BindAction<FUseDelegate>("Potion1", EInputEvent::IE_Pressed, this, &ABasePlayableCharacter::UsePotionAtIndex, 0);
	PlayerInputComponent->BindAction<FUseDelegate>("Potion2", EInputEvent::IE_Pressed, this, &ABasePlayableCharacter::UsePotionAtIndex, 1);

	// Reset camera action bind
	PlayerInputComponent->BindAction("ResetCamera", EInputEvent::IE_Pressed, this, &ABasePlayableCharacter::ResetCamera);

	// Use action bind
	PlayerInputComponent->BindAction("Use", EInputEvent::IE_Pressed, this, &ABasePlayableCharacter::Use);

	// Open and close inventory UI action bind
	PlayerInputComponent->BindAction("Inventory", EInputEvent::IE_Pressed, this, &ABasePlayableCharacter::OpenCloseInventoryUI);
	#pragma endregion
}
#pragma endregion
#pragma endregion

#pragma region Enemy
ABaseEnemy* ABasePlayableCharacter::GetFocusEnemy()
{
	return FocusEnemy;
}

void ABasePlayableCharacter::SetFocusEnemy(class ABaseEnemy* NewFocusEnemy)
{
	FocusEnemy = NewFocusEnemy;
}
#pragma endregion

#pragma region Combat
#pragma region Damage
void ABasePlayableCharacter::Die()
{
	//Super::Die();

	// Stop movement immeditely
	GetCharacterMovement()->StopMovementImmediately();

	// Disable input
	ATDFPlayerController* PlayerController = Cast<ATDFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
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
bool ABasePlayableCharacter::HasRange(float Range, ESkillType Type)
{
	// Get player controller
	ATDFPlayerController* PlayerController = Cast<ATDFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
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

void ABasePlayableCharacter::ResetAutoAttack()
{
	BasicAttackCooldown = 0.f;
}

void ABasePlayableCharacter::BasicAttack(ABaseEnemy* EnemyReference)
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

void ABasePlayableCharacter::UseSkill(int Index)
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
FTransform ABasePlayableCharacter::GetSkillSpawnTransform(ESkillSpawnOption SpawnOption)
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

void ABasePlayableCharacter::AttachSkill(ESkillSpawnOption SpawnOption)
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

void ABasePlayableCharacter::SpawnHit()
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
void ABasePlayableCharacter::AddMoney(int CoinsToAdd, int GemsToAdd)
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

void ABasePlayableCharacter::SetWalletUI(int AddedCoins, int AddedGems)
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
bool ABasePlayableCharacter::HasTemporary()
{
	return (TempItem != nullptr || TempChest != nullptr);
}

void ABasePlayableCharacter::SetTempItem(ABaseItem* Value)
{
	TempItem = Value;
}

void ABasePlayableCharacter::SetTempChest(class ABaseChest* Value)
{
	TempChest = Value;
}
#pragma endregion

#pragma region Update
void ABasePlayableCharacter::UpdateCooldowns(float DeltaSeconds)
{
	// Update cooldowns (on super class)
	Super::UpdateCooldowns(DeltaSeconds);

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
}
#pragma endregion

#pragma region Gamepad
void ABasePlayableCharacter::ForwardMovement(float Value)
{
	// Set YIn variable
	YIn = Value;

	// Fix foward axis diagonal for gamepad
	float ClampValue = UKismetMathLibrary::MapRangeClamped(UKismetMathLibrary::Abs(XIn), 0.f, 0.6f, 1.f, 1.2f) * YIn;
	YOut = UKismetMathLibrary::FClamp(ClampValue, -1.f, 1.f);

	// Call movement input method
	MovementInput(true);
}

void ABasePlayableCharacter::RightMovement(float Value)
{
	// Set XIn variable
	XIn = Value;

	// Fix right axis diagonal for gamepad
	float ClampValue = UKismetMathLibrary::MapRangeClamped(UKismetMathLibrary::Abs(YIn), 0.f, 0.6f, 1.f, 1.2f) * XIn;
	XOut = UKismetMathLibrary::FClamp(ClampValue, -1.f, 1.f);

	// Call movement input method
	MovementInput(false);
}

void ABasePlayableCharacter::MovementInput(bool bIsForwardMovement)
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

void ABasePlayableCharacter::GamepadBasicHit()
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
void ABasePlayableCharacter::CameraZoom(float Value)
{
	// If there is no input, exit
	if(Value == 0.f)
	{
		return;
	}
	
	// Get player controller
	ATDFPlayerController* PlayerController = Cast<ATDFPlayerController>
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

void ABasePlayableCharacter::ResetCamera()
{
	CameraBoom->TargetArmLength = 2200.f;
}
#pragma endregion

#pragma region Use
void ABasePlayableCharacter::Use()
{
	UE_LOG(LogTemp, Error, TEXT("Use called!"));

	// If there is any temporary item or chest
	if(HasTemporary())
	{
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

	// If there is any interaction
	if(HasInteractions())
	{
		Interact();
	}
	
}
#pragma endregion

#pragma region Potions
void ABasePlayableCharacter::UsePotionAtIndex(int Index)
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
void ABasePlayableCharacter::OpenCloseInventoryUI()
{
	// If there is an inventory reference, call the OpenInventoryUI method
	if(Inventory)
	{
		Inventory->OpenInventoryUI();
	}
}
#pragma endregion

#pragma region Wallet
void ABasePlayableCharacter::SetCoins(int Value)
{
	Coins = Value;
}

void ABasePlayableCharacter::SetGems(int Value)
{
	Gems = Value;
}

int ABasePlayableCharacter::GetCoins()
{
	return Coins;
}

int ABasePlayableCharacter::GetGems()
{
	return Gems;
}
#pragma endregion
