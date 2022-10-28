// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "BaseEnemy.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#pragma endregion


#pragma region Constructor
// Sets default values
ABaseEnemy::ABaseEnemy() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
