// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "DamageSystem.h"
#include "BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#pragma endregion


#pragma region Constructor
// Sets default values
ADamageSystem::ADamageSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
#pragma endregion

#pragma region Events
#pragma region Begin Play
// Called when the game starts or when spawned
void ADamageSystem::BeginPlay()
{
	Super::BeginPlay();

}
#pragma endregion

#pragma region Tick
// Called every frame
void ADamageSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#pragma endregion
#pragma endregion

#pragma region Damage
int ADamageSystem::CalculateTakenDamage(ADamageSystem* EnemyDamageSystem)
{
	// Setup enemy damage local variables
	int EnemyAttackDamage = EnemyDamageSystem->AttackDamage;
	int EnemyAbilityPower = EnemyDamageSystem->AbilityPower;
	int EnemyTrueDamage = EnemyDamageSystem->TrueDamage;
	int EnemyPoisonDamage = EnemyDamageSystem->PoisonDamage;
	int EnemyFireDamage = EnemyDamageSystem->FireDamage;
	int EnemyLightningDamage = EnemyDamageSystem->LightningDamage;
	int EnemyBleedingDamage = EnemyDamageSystem->BleedingDamage;

	// Setup final damage local variables
	int FinalAttackDamage = UKismetMathLibrary::Clamp((EnemyAttackDamage - PhysicalArmor), 0, EnemyAttackDamage);
	int FinalAbilityPower = UKismetMathLibrary::Clamp((EnemyAbilityPower - MagicArmor), 0, EnemyAbilityPower);
	int FinalPoisonDamage = UKismetMathLibrary::Clamp((EnemyPoisonDamage - PoisonArmor), 0, EnemyPoisonDamage);
	int FinalFireDamage = UKismetMathLibrary::Clamp((EnemyFireDamage - FireArmor), 0, EnemyFireDamage);
	int FinalLightningDamage = UKismetMathLibrary::Clamp((EnemyLightningDamage - LightningArmor), 0, EnemyLightningDamage);
	int FinalBleedingDamage = UKismetMathLibrary::Clamp((EnemyBleedingDamage - BleedingArmor), 0, EnemyBleedingDamage);

	// Set all status
	PoisonStatus = UKismetMathLibrary::Clamp((PoisonStatus + FinalPoisonDamage), 0, MaxPoisonStatus);
	FireStatus = UKismetMathLibrary::Clamp((FireStatus + FinalFireDamage), 0, MaxFireStatus);
	LightningStatus = UKismetMathLibrary::Clamp((LightningStatus + FinalLightningDamage), 0, MaxLightningStatus);
	BleedingStatus = UKismetMathLibrary::Clamp((BleedingStatus + FinalBleedingDamage), 0, MaxBleedingStatus);

	// Check all new status
	CheckAllStatus();

	// Return final damage
	return (FinalAttackDamage +
			FinalAbilityPower +
			EnemyTrueDamage +
			FinalPoisonDamage +
			FinalFireDamage +
			FinalLightningDamage +
			FinalBleedingDamage);
}
#pragma endregion

#pragma region Status
void ADamageSystem::Poison()
{
	GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Cyan, TEXT("Poison damage!"));
	// If there is no character reference set, exit
	if(!Character)
	{
		GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Red, TEXT("No character!"));
		return;
	}

	// Clear timer and decrease poison status
	GetWorldTimerManager().ClearTimer(PoisonTimer);
	PoisonStatus--;
	Character->DecreaseHealth(2);

	// If poison status is less or equal 0, reset to 0
	if(PoisonStatus <= 0)
	{
		PoisonStatus = 0;
	}

	// If poison status is bigger than 0, call Poison method after 0.3 second
	else
	{
		GetWorldTimerManager().SetTimer(PoisonTimer, this, &ADamageSystem::Poison, 1.5f);
	}
}

void ADamageSystem::Fire()
{
}

void ADamageSystem::Lightning()
{
}

void ADamageSystem::Bleeding()
{
}

void ADamageSystem::CheckAllStatus()
{
	// Poison status
	if(PoisonStatus >= MaxPoisonStatus)
	{
		Poison();
	}

	// Fire status
	if(FireStatus >= MaxFireStatus)
	{
		Fire();

	}

	// Lightning status
	if(LightningStatus >= MaxLightningStatus)
	{
		Lightning();
	}

	// Bleeding status
	if(BleedingStatus >= MaxBleedingStatus)
	{
		Bleeding();
	}
}

void ADamageSystem::AddStatus(int AddedPoisonDamage, int AddedFireDamage, int AddedLightningDamage, int AddedBleedingDamage)
{
	// Set all status
	PoisonStatus = UKismetMathLibrary::Clamp((PoisonStatus + AddedPoisonDamage), 0, MaxPoisonStatus);
	FireStatus = UKismetMathLibrary::Clamp((FireStatus + AddedFireDamage), 0, MaxFireStatus);
	LightningStatus = UKismetMathLibrary::Clamp((LightningStatus + AddedLightningDamage), 0, MaxLightningStatus);
	BleedingStatus = UKismetMathLibrary::Clamp((BleedingStatus + AddedBleedingDamage), 0, MaxBleedingStatus);

	// Check all new status
	CheckAllStatus();
}
#pragma endregion

#pragma region Character
void ADamageSystem::SetCharacter(ABaseCharacter* NewCharacter)
{
	Character = NewCharacter;
}
#pragma endregion
