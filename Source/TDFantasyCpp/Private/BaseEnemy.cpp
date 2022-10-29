// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "BaseEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#pragma endregion


#pragma region Constructor
// Sets default values
ABaseEnemy::ABaseEnemy() {
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
void ABaseEnemy::BeginPlay() {
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
void ABaseEnemy::TakeHit(int PlayerAttackDamage, int PlayerAbilityPower, int PlayerTrueDamage)
{
	// If player attack damage is bigger than enemy's attack armor
	if(PlayerAttackDamage > AttackArmor)
	{
		// Decrease health and add temporary damage suffered
		Health -= PlayerAttackDamage - AttackArmor;
		TempDamageSuffered += PlayerAttackDamage - AttackArmor;
	}

	// If player ability power is bigger than enemy's magic armor
	if(PlayerAbilityPower > MagicArmor)
	{
		// Decrease health and add temporary damage suffered
		Health -= PlayerAbilityPower - MagicArmor;
		TempDamageSuffered += PlayerAbilityPower - MagicArmor;
	}

	// Decrease health and add temporary damage suffered, based on true damage
	Health -= PlayerTrueDamage;
	TempDamageSuffered += PlayerTrueDamage;

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

	// If health is less or equal 0 and enemy is not dead, kill it
	if(Health <= 0 && !bIsDead)
	{
		Die();
	}
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

#pragma region Getters
bool ABaseEnemy::GetIsDead()
{
	return bIsDead;
}
#pragma endregion
