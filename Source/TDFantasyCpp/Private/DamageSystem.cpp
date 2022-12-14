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

	// Divide all poison damage percents
	PoisonDamagePercent /= 100.f;
	FireDamagePercent /= 100.f;
	LightningDamagePercent /= 100.f;
	BleedingDamagePercent /= 100.f;
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
	AddStatus
	(
		(PoisonStatus + FinalPoisonDamage), 
		(FireStatus + FinalFireDamage), 
		(LightningStatus + FinalLightningDamage), 
		(BleedingStatus + FinalBleedingDamage)
	);
	/*PoisonStatus = UKismetMathLibrary::Clamp((PoisonStatus + FinalPoisonDamage), 0, MaxPoisonStatus);
	FireStatus = UKismetMathLibrary::Clamp((FireStatus + FinalFireDamage), 0, MaxFireStatus);
	LightningStatus = UKismetMathLibrary::Clamp((LightningStatus + FinalLightningDamage), 0, MaxLightningStatus);
	BleedingStatus = UKismetMathLibrary::Clamp((BleedingStatus + FinalBleedingDamage), 0, MaxBleedingStatus);*/

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
	// If there is no character reference set, exit
	if(!Character)
	{
		return;
	}

	// Clear timer and decrease poison status
	GetWorldTimerManager().ClearTimer(PoisonTimer);
	PoisonStatus--;
	Character->DecreaseHealth(Character->BaseHealth * PoisonDamagePercent);

	// If poison status is less or equal 0, reset it to 0
	if(PoisonStatus <= 0)
	{
		bIsPoisonStatusActive = false;
		PoisonStatus = 0;
	}

	// If poison status is bigger than 0, call Poison method after 1.5 second
	else
	{
		GetWorldTimerManager().SetTimer(PoisonTimer, this, &ADamageSystem::Poison, 1.5f);
	}
}

void ADamageSystem::Fire()
{
	// If there is no character reference set, exit
	if(!Character)
	{
		return;
	}

	// Clear timer and decrease fire status
	GetWorldTimerManager().ClearTimer(FireTimer);
	FireStatus -= 10;
	Character->DecreaseHealth(Character->BaseHealth * FireDamagePercent);

	// If fire status is less or equal 0, reset it to 0
	if(FireStatus <= 0)
	{
		bIsFireStatusActive = false;
		FireStatus = 0;
	}

	// If fire status is bigger than 0, call Fire method after 0.6 second
	else
	{
		GetWorldTimerManager().SetTimer(FireTimer, this, &ADamageSystem::Fire, 0.6f);
	}
}

void ADamageSystem::Lightning()
{
	// If there is no character reference set, exit
	if(!Character)
	{
		return;
	}

	// Clear timer and decrease fire status
	GetWorldTimerManager().ClearTimer(LightningTimer);

	// If lightning status is bigger than 0, give a shock damage
	if(LightningStatus > 0)
	{
		LightningStatus -= 35;
		Character->DecreaseHealth(Character->BaseHealth * LightningDamagePercent * UKismetMathLibrary::RandomFloatInRange(0.5f, 2.f));
		GetWorldTimerManager().SetTimer(LightningTimer, this, &ADamageSystem::Lightning, 1.5f);
	}

	// If lightning status is less or equal 0, reset it to 0
	else
	{
		bIsLightningStatusActive = false;
		LightningStatus = 0;
	}
}

void ADamageSystem::Bleeding()
{
	// If there is no character reference set, exit
	if(!Character)
	{
		return;
	}

	// If bleeding status is bigger or equal MaxBleedingStatus (do it once)
	if(BleedingStatus >= MaxBleedingStatus)
	{
		Character->DecreaseHealth(Character->BaseHealth * BleedingDamagePercent);
	}
	
	// Decrease bleeding status by 5
	BleedingStatus -= 5;
	
	// If bleeding status is less or equal 0, reset it to 0
	if(BleedingStatus <= 0)
	{
		bIsBleedingStatusActive = false;
		BleedingStatus = 0;
	}

	// If bleeding status is bigger than 0, call Bleeding method after 1 second
	else
	{
		GetWorldTimerManager().SetTimer(BleedingTimer, this, &ADamageSystem::Bleeding, 1.f);
	}
}

void ADamageSystem::CheckAllStatus()
{
	// Poison status
	if(PoisonStatus >= MaxPoisonStatus)
	{
		bIsPoisonStatusActive = true;
		Poison();
	}

	// Fire status
	if(FireStatus >= MaxFireStatus)
	{
		bIsFireStatusActive = true;
		Fire();

	}

	// Lightning status
	if(LightningStatus >= MaxLightningStatus)
	{
		bIsLightningStatusActive = true;
		Lightning();
	}

	// Bleeding status
	if(BleedingStatus >= MaxBleedingStatus)
	{
		bIsBleedingStatusActive = true;
		Bleeding();
	}
}

void ADamageSystem::ClearStatus(int StatusToClear, FTimerHandle Timer)
{
	// Clear timer
	GetWorldTimerManager().ClearTimer(Timer);

	// If any status is active, exit
	if(bIsPoisonStatusActive || bIsFireStatusActive || bIsLightningStatusActive || bIsBleedingStatusActive)
	{
		return;
	}

	// Switch on status
	switch(StatusToClear)
	{
		// Poison status
		case 0:
			PoisonStatus -= 5;
			break;

		// Fire status
		case 1:
			FireStatus -= 5;
			break;

		// Lightning status
		case 2:
			LightningStatus -= 5;
			break;

		// Bleeding status
		case 3:
			BleedingStatus -= 5;
			break;
	}

	// If all status are less or equal to 0, reset all and exit
	if(PoisonStatus <= 0 && FireStatus <= 0 && LightningStatus <= 0 && BleedingStatus <= 0)
	{
		PoisonStatus = 0;
		FireStatus = 0;
		LightningStatus = 0;
		BleedingStatus = 0;
		return;
	}

	// Call ClearStatus after 2 seconds
	FTimerDelegate PoisonTimerDelegate;
	PoisonTimerDelegate.BindUFunction(this, FName("ClearStatus"), StatusToClear, Timer);
	GetWorldTimerManager().SetTimer(Timer, PoisonTimerDelegate, 3.f, false);
}

void ADamageSystem::AddStatus(int AddedPoisonDamage, int AddedFireDamage, int AddedLightningDamage, int AddedBleedingDamage)
{
	// Setup local variables
	int StatusToClear = -1;
	FTimerHandle Timer;

	// If poison status is not active, add poison status
	if(!bIsPoisonStatusActive && AddedPoisonDamage > 0)
	{
		GEngine->AddOnScreenDebugMessage(14, 5.f, FColor::Red, TEXT("POISON DAMAGE"));
		GetWorldTimerManager().ClearTimer(PoisonTimer);

		PoisonStatus = UKismetMathLibrary::Clamp((PoisonStatus + AddedPoisonDamage), 0, MaxPoisonStatus);

		StatusToClear = 0;
		Timer = PoisonTimer;
	}
	
	// If fire status is not active, add fire status
	if(!bIsFireStatusActive && AddedFireDamage > 0)
	{
		GetWorldTimerManager().ClearTimer(FireTimer);

		FireStatus = UKismetMathLibrary::Clamp((FireStatus + AddedFireDamage), 0, MaxFireStatus);

		StatusToClear = 1;
		Timer = FireTimer;
	}
	
	// If lightning status is not active, add lightning status
	if(!bIsLightningStatusActive && AddedLightningDamage > 0)
	{
		GetWorldTimerManager().ClearTimer(LightningTimer);

		LightningStatus = UKismetMathLibrary::Clamp((LightningStatus + AddedLightningDamage), 0, MaxLightningStatus);

		StatusToClear = 2;
		Timer = LightningTimer;
	}
	
	// If bleeding status is not active, add bleeding status
	if(!bIsBleedingStatusActive && AddedBleedingDamage > 0)
	{
		GetWorldTimerManager().ClearTimer(BleedingTimer);

		BleedingStatus = UKismetMathLibrary::Clamp((BleedingStatus + AddedBleedingDamage), 0, MaxBleedingStatus);

		StatusToClear = 3;
		Timer = BleedingTimer;
	}
	

	// Check all new status
	CheckAllStatus();

	// If there are status to clear, call ClearStatus after 3 seconds
	if(StatusToClear != -1)
	{
		FTimerDelegate PoisonTimerDelegate;
		PoisonTimerDelegate.BindUFunction(this, FName("ClearStatus"), StatusToClear, Timer);
		GetWorldTimerManager().SetTimer(Timer, PoisonTimerDelegate, 3.f, false);
	}	
}
#pragma endregion

#pragma region Character
void ADamageSystem::SetCharacter(ABaseCharacter* NewCharacter)
{
	Character = NewCharacter;
}
#pragma endregion
