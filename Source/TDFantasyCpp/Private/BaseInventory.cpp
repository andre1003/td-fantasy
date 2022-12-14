// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "BaseInventory.h"
#include "Blueprint/UserWidget.h"
#include "BaseItem.h"
#include "BaseInventorySlot.h"
#include "Misc/OutputDeviceNull.h"
#include "Kismet/GameplayStatics.h"
#pragma endregion


#pragma region Constructor
// Sets default values
ABaseInventory::ABaseInventory()
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
void ABaseInventory::BeginPlay()
{
	Super::BeginPlay();

	// Initialize slots array
	Slots = TArray<ABaseInventorySlot*>();
}
#pragma endregion

#pragma region Tick
// Called every frame
void ABaseInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#pragma endregion
#pragma endregion

#pragma region Methods
void ABaseInventory::AddItem(TSubclassOf<class ABaseItem> NewItem, int Amount)
{
	// Set added item
	AddedItem = NewItem;

	// Add new item UI
	AddUI(Amount);

	bool bHasItem = HasItem(NewItem);

	// If there is already an item of this type on inventory, add its amount
	if(bHasItem)
	{
		Slots[SlotIndex]->IncreaseAmount(Amount);
		SlotIndex = -1;
	}

	// If there is not this item on inventory
	else
	{
		// Spawn new slot
		ABaseInventorySlot* NewSlot = GetWorld()->SpawnActor<ABaseInventorySlot>();

		// Add item info to it
		NewSlot->Add(NewItem, Amount, this);

		// Add new slot to slots array
		Slots.Add(NewSlot);
	}
}

void ABaseInventory::RemoveItem(TSubclassOf<class ABaseItem> ItemToRemove, int Amount)
{
	if(HasItem(ItemToRemove))
	{
		Slots[SlotIndex]->DecreaseAmount(Amount);
	}
}

bool ABaseInventory::HasItem(TSubclassOf<class ABaseItem> SearchItem)
{
	int SlotsLength = Slots.Num();

	for(int i = 0; i < SlotsLength; i++)
	{
		if(HasItemOnSlot(Slots[i], SearchItem))
		{
			SlotIndex = i;
			return true;
		}
	}
	
	SlotIndex = -1;
	return false;
}

void ABaseInventory::OpenInventoryUI()
{
	// If UI is already being displayed, close it
	if(InventoryUI)
	{
		FOutputDeviceNull OutputDevice;
		InventoryUI->CallFunctionByNameWithArguments(TEXT("RemoveUI"), OutputDevice, nullptr, true);
		InventoryUI = nullptr;
	}

	// If there is no inventory UI, open it
	else
	{
		InventoryUI = CreateWidget<UUserWidget>(GetWorld(), InventoryUIClass);
		InventoryUI->AddToViewport();
	}
}
#pragma endregion
