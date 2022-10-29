// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "BaseChest.h"
#include "BaseInventory.h"
#include "Kismet/GameplayStatics.h"
#include "TDFantasyCpp/BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#pragma endregion


#pragma region Constructor
// Sets default values
ABaseChest::ABaseChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup root component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComponent"));
	RootComponent = RootSceneComponent;
}
#pragma endregion

#pragma region Events
#pragma region Begin Play
// Called when the game starts or when spawned
void ABaseChest::BeginPlay()
{
	Super::BeginPlay();

}

#pragma endregion

#pragma region Tick
// Called every frame
void ABaseChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma endregion

#pragma region Overlap
void ABaseChest::OverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
}

void ABaseChest::OverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
}
#pragma endregion
#pragma endregion

#pragma region Methods
void ABaseChest::CollectItems()
{
	// Loop reward items
	int ItemsLength = Items.Num();
	for(int i = 0; i < ItemsLength; i++)
	{
		// Add each item to player inventory and remove it from this list
		PlayerInventory->AddItem(Items[i], ItemsAmount[i]);
		Items.RemoveAt(i);
		ItemsAmount.RemoveAt(i);
	}

	// Get player
	ABaseCharacter* Player = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Randomize coins and gems and add then to player's wallet
	int CoinsToAdd = UKismetMathLibrary::RandomIntegerInRange(0, MaxCoins);
	int GemsToAdd = UKismetMathLibrary::RandomIntegerInRange(0, MaxGems);
	Player->AddMoney(CoinsToAdd, GemsToAdd);

	// Add XP
	//
}
#pragma endregion
