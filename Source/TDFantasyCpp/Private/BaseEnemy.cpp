// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "BaseEnemy.h"
#include "BaseCharacter.h"
#include "BasePlayableCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "DamageSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSystem.h"
#pragma endregion


#pragma region Constructor
// Sets default values
ABaseEnemy::ABaseEnemy() 
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthUI"));
	HealthUI->SetWidgetSpace(EWidgetSpace::Screen);
	HealthUI->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
	HealthUI->SetupAttachment(RootComponent);
}
#pragma endregion

#pragma region Events
#pragma region Begin Play	
// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay() 
{
	Super::BeginPlay();

	// Setup PlayerReference
	PlayerReference = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Setup UI (enemy health bar)
	SetupUI();

	// Call RageController method
	RageController();
}
#pragma endregion

#pragma region Tick
// Called every frame
void ABaseEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// Update enemy's movement
	UpdateMovement();
}
#pragma endregion

#pragma region Setup Input
// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
#pragma endregion
#pragma endregion

#pragma region Rage
void ABaseEnemy::RageController() {
	// Clear RageTimer
	GetWorldTimerManager().ClearTimer(RageTimer);

	// If enemy can rage and is not dead
	if(bCanRage && !bIsDead) {
		// Set random bool to bIsAngry
		bIsAngry = FMath::RandBool();

		// Set rage effect
		RageEffect();

		// If is angry, wait for rage cooldown to calculate this again
		if(bIsAngry)
			GetWorldTimerManager().SetTimer(RageTimer, this, &ABaseEnemy::RageController, RageCooldown);
		// If not, wait 10 seconds
		else
			GetWorldTimerManager().SetTimer(RageTimer, this, &ABaseEnemy::RageController, 10.f);
	}
}
#pragma endregion

#pragma region Take Damage
void ABaseEnemy::TakeHit(ADamageSystem* EnemyDamageSystem, int PoisonDamage, int FireDamage, int LightningDamage, int BleedingDamage)
{
	// Get previous enemy health
	int PrevHealth = Health;

	// Call parent TakeHit method
	Super::TakeHit(EnemyDamageSystem, PoisonDamage, FireDamage, LightningDamage, BleedingDamage);

	// Calculate temporary damage suffered
	TempDamageSuffered += PrevHealth - Health;

	// Play hit animation
	PlayAnimMontage(Hit);

	// Hit effects
	DamageEffect();
	SlowDown();

	// Chase player and call friends
	ChasePlayer();
	CallFriends();

	// Call clear temporary damage timer
	ClearTempDamage();
}

void ABaseEnemy::DecreaseHealth(int HealthToRemove)
{
	// Execute base DecreaseHealth method
	Super::DecreaseHealth(HealthToRemove);

	// Calculate temporary damage suffered
	TempDamageSuffered += HealthToRemove;

	// Call clear temporary damage timer
	ClearTempDamage();
}

void ABaseEnemy::Die()
{
	// Set is dead to true
	bIsDead = true;

	// Remove all collisions
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	
	// Destroy enemy health bar
	HealthUI->DestroyComponent();

	// Set life span to 15 seconds
	SetLifeSpan(15.f);

	// Start death fade effect
	DeathEffect();

	// Giver rewards to the player
	GiveRewards();
}
#pragma endregion

#pragma region Enemy Movement
void ABaseEnemy::UpdateMovement() {
	// If enemy is not dead
	if(!bIsDead) {
		// If enemy cannot see player (enemy of player's enemy), patrol
		if(!bCanSeeEnemy) {
			Patrol();
		}
		// If it can see player
		else {
			// If has to stop (player died or distance is bigger than MaxDistanceFromEnemy), set bCanSee to false
			if(StopChasePlayer()) {
				bCanSeeEnemy = false;
			}
			// Else, chase player
			else {
				ChasePlayer();
			}
		}
	}
	// If enemy is dead
	else {
		// Stop movement immeditely
		GetCharacterMovement()->StopMovementImmediately();
	}
}
#pragma endregion

#pragma region Combat
void ABaseEnemy::StartAttack()
{
	// If attack is on cooldown or enemy is dead, exit
	if(BasicAttackCooldown > 0.f || bIsDead)
	{
		return;
	}

	// Set is attacking to true
	bIsAttacking = true;

	// Setup standard play animation
	UAnimMontage* EnemyAttackAnimation = AttackLeft;
	float CooldownDivisor = 1.f;
	float PlayRate = 1.2f;

	// If enemy is angry
	if(bIsAngry)
	{
		// If he has right attacked, change attack animation to right attack
		if(!bHasRightAttacked)
		{
			EnemyAttackAnimation = AttackRight;
		}

		// Switch bHasRightAttacked and change play animation settings
		bHasRightAttacked = !bHasRightAttacked;
		CooldownDivisor = 2.f;
		PlayRate = 2.2f;
	}

	// Play animation and get attack cooldown
	BasicAttackCooldown = PlayAnimMontage(EnemyAttackAnimation, PlayRate) / CooldownDivisor;

	// Start attack cooldown
	StartAttackCooldown();
}

void ABaseEnemy::DoDamage(AActor* Actor)
{
	// If enemy is not attacking, exit
	if(!bIsAttacking || bIsDead)
	{
		return;
	}

	// Set is attacking to false
	bIsAttacking = false;

	// Cast to player base character
	ABasePlayableCharacter* Player = Cast<ABasePlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// If cast failed, exit
	if(!Player)
	{
		return;
	}

	// Damage the player
	Player->TakeHit(DamageSystem);
}
#pragma endregion

#pragma region Rewards
void ABaseEnemy::GiveRewards()
{
	// Get player character
	ABasePlayableCharacter* Player = Cast<ABasePlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Calculate chance of give coins and gems
	float CoinChance = FMath::RandRange(0.f, 1.f);
	float GemChance = FMath::RandRange(0.f, 1.f);

	// Initial coins and gems values
	int _Coins = 0;
	int _Gems = 0;

	// If coin chance is in coin percent range, set the coins value to default
	if(CoinChance <= CoinPercent)
	{
		_Coins = Coins;
	}

	// If gem chance is in gem percent range, set the gems value to default
	if(GemChance <= GemPercent)
	{
		_Gems = Gems;
	}

	// Add money and experience to player
	Player->AddMoney(_Coins, _Gems);
	Player->Level->AddXP(XP);
}
#pragma endregion

#pragma region Getters
bool ABaseEnemy::GetIsDead()
{
	return bIsDead;
}
#pragma endregion
