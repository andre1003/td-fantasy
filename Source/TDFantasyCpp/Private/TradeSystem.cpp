// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "TradeSystem.h"
#include "BaseCharacter.h"
#include "BaseInventory.h"
#include "BaseInventorySlot.h"
#include "BaseItem.h"
#include "BasePlayableCharacter.h"
#include "Kismet/GameplayStatics.h"
#pragma endregion


#pragma region Constructor
// Sets default values
ATradeSystem::ATradeSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
#pragma endregion

#pragma region Events
#pragma region BeginPlay
// Called when the game starts or when spawned
void ATradeSystem::BeginPlay()
{
	Super::BeginPlay();

	// Get player reference
	Player = Cast<ABasePlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}
#pragma endregion

#pragma region Tick
// Called every frame
void ATradeSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#pragma endregion
#pragma endregion

void ATradeSystem::SetNPC(ABaseCharacter* NewNPC)
{
	NPC = NewNPC;
}

void ATradeSystem::BuyItem(int ItemIndex, int ItemAmount)
{
	// Get item
	TSubclassOf<ABaseItem> ItemClass = NPC->Inventory->Slots[ItemIndex]->GetItem();
	int BuyPrice = GetItemBuyPrice(ItemClass);

	// If item's buy price is bigger than player's coins or NPC does not have enought items, exit
	if(BuyPrice > Player->GetCoins() || NPC->Inventory->Slots[ItemIndex]->GetAmount() < ItemAmount) {
		return;
	}

	// Add item to player inventory and remove from NPC inventory
	Player->Inventory->AddItem(ItemClass, ItemAmount);
	NPC->Inventory->RemoveItem(ItemClass, ItemAmount);

	// Pay the item price
	Player->SetCoins(Player->GetCoins() - BuyPrice);
}

void ATradeSystem::SellItem(int ItemIndex, int ItemAmount)
{
	// Get item
	TSubclassOf<ABaseItem> ItemClass = NPC->Inventory->Slots[ItemIndex]->GetItem();
	int SellPrice = GetItemSellPrice(ItemClass);

	// If player does not have enought items, exit
	if(Player->Inventory->Slots[ItemIndex]->GetAmount() < ItemAmount) {
		return;
	}

	// Add item to NPC inventory and remove from player inventory
	NPC->Inventory->AddItem(ItemClass, ItemAmount);
	Player->Inventory->RemoveItem(ItemClass, ItemAmount);

	// Add coins to player
	Player->SetCoins(Player->GetCoins() + SellPrice);
}

