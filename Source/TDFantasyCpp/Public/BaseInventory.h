// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.

#pragma once

#pragma region Includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseInventory.generated.h"
#pragma endregion


UCLASS()
class TDFANTASYCPP_API ABaseInventory : public AActor
{
	GENERATED_BODY()
	

#pragma region Attributes
	#pragma region Public Attributes
public:
	#pragma region Slots
	// Slot array
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Slots")
		TArray<class ABaseInventorySlot*> Slots;
	#pragma endregion

	// Root scene component
	class USceneComponent* RootSceneComponent;
	#pragma endregion

	#pragma region Protected Attributes
protected:
	#pragma region UI
	//////// THIS SAVED ME  ////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")  //
		TSubclassOf<class ABaseItem> AddedItem;					  //
	////////////////////////////////////////////////////////////////

	// Inventory UI widget class reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UUserWidget> InventoryUIClass;

	// Inventory UI widget
	class UUserWidget* InventoryUI;
	#pragma endregion
	#pragma endregion

	#pragma region Private Attributes
private:
	// Slot array index
	int SlotIndex = -1;
	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	// Sets default values for this actor's properties
	ABaseInventory();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Add some new items to inventory.
		/// </summary>
		/// <param name="NewItem">- New item to add.</param>
		/// <param name="Amount">- Amount to add.</param>
		void AddItem(TSubclassOf<class ABaseItem> NewItem, int Amount);
	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Remove an item from inventory.
		/// </summary>
		/// <param name="ItemToRemove">- Item to be removed.</param>
		/// <param name="Amount">- Amount to be removed.</param>
		void RemoveItem(TSubclassOf<class ABaseItem> ItemToRemove, int Amount);

	/// <summary>
	/// Check if a given item exists on inventory.
	/// </summary>
	/// <param name="SearchItem">- Item to search.</param>
	/// <returns>Check result. TRUE if there is an item on inventory, FALSE if not.</returns>
	bool HasItem(TSubclassOf<class ABaseItem> SearchItem);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Add new item UI widget.
		/// </summary>
		/// <param name="Amount">- Amount added.</param>
		void AddUI(int Amount);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Check if this slot has the searched item.
		/// </summary>
		/// <param name="OtherItem">Item to compare.</param>
		/// <returns>This slot item equal to OtherItem. TRUE if it is, FALSE if it is not.</returns>
		bool HasItemOnSlot(class ABaseInventorySlot* Slot, TSubclassOf<class ABaseItem> OtherItem);

	/// <summary>
	/// Open the inventory UI.
	/// </summary>
	void OpenInventoryUI();
	#pragma endregion

	#pragma region Protected Methods
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	#pragma endregion

	#pragma region Private Methods

	#pragma endregion
#pragma endregion
};
