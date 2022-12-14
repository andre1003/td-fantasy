// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "BasePotion.h"
#include "BasePlayableCharacter.h"
#include "Kismet/GameplayStatics.h"
#pragma endregion


#pragma region Contructor
// Sets default values
ABasePotion::ABasePotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
#pragma endregion

#pragma region Events
#pragma region Begin Play
// Called when the game starts or when spawned
void ABasePotion::BeginPlay()
{
	Super::BeginPlay();

}
#pragma endregion

#pragma region Tick
// Called every frame
void ABasePotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#pragma endregion
#pragma endregion

#pragma region Methods
void ABasePotion::UsePotion_Implementation()
{
	// If cannot use this potion, exit
	if(!bCanUsePotion)
	{
		return;
	}

	// Get player character
	ABasePlayableCharacter* Player = Cast<ABasePlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Restore stats
	Player->RestoreHealthAndMana(HealthRestore, ManaRestore);

	// Start potion cooldown
	bCanUsePotion = false;
	GetWorldTimerManager().SetTimer(CooldownTimer, this, &ABasePotion::ResetCooldown, Cooldown, false);
}

void ABasePotion::ResetCooldown()
{
	// Clear timer and allow to use this potion again
	GetWorldTimerManager().ClearTimer(CooldownTimer);
	bCanUsePotion = true;
}
#pragma endregion
