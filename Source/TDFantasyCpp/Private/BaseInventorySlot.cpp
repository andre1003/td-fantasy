// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "BaseInventorySlot.h"
#include "BaseInventory.h"
#include "BaseItem.h"
#pragma endregion


#pragma region Constructor
// Sets default values
ABaseInventorySlot::ABaseInventorySlot()
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
void ABaseInventorySlot::BeginPlay()
{
	Super::BeginPlay();

}
#pragma endregion

#pragma region Tick
// Called every frame
void ABaseInventorySlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#pragma endregion
#pragma endregion

#pragma region Methods
TSubclassOf<class ABaseItem> ABaseInventorySlot::GetItem()
{
	// Return slot item
	return Item;
}

int ABaseInventorySlot::GetAmount()
{
	return Amount;
}

void ABaseInventorySlot::IncreaseAmount(int AmountToAdd)
{
	// Increase amount
	Amount += AmountToAdd;
}

void ABaseInventorySlot::DecreaseAmount(int AmountToDecrease)
{
	// Decrease amount
	Amount -= AmountToDecrease;

	// If amount is less or equal to 0, remove this slot from inventory
	if(Amount <= 0)
	{
		Remove();
	}
}

void ABaseInventorySlot::Add(TSubclassOf<class ABaseItem> ItemReference, int ItemAmount, ABaseInventory* InventoryReference)
{
	// Set item
	Item = ItemReference;

	// Set Amount
	Amount = ItemAmount;

	// Set is empty controller
	bIsEmpty = false;

	// Set inventory reference
	Inventory = InventoryReference;

	// Attach to inventory
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::FAttachmentTransformRules
	(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);
	AttachToActor(Inventory, AttachmentRules);
}

void ABaseInventorySlot::Remove()
{
	// Remove this slot from inventory
	Inventory->Slots.Remove(this);

	// Destroy this inventory slot
	Destroy();
}
#pragma endregion
