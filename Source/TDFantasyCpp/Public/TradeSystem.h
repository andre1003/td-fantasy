// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TradeSystem.generated.h"

UCLASS()
class TDFANTASYCPP_API ATradeSystem : public AActor
{
	GENERATED_BODY()
	
#pragma region Attributes
	#pragma region Public Attributes

	#pragma endregion

	#pragma region Protected Attributes
protected:
	// NPC reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traders")
		class ABaseCharacter* NPC;

	// Player reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Traders")
		class ABasePlayableCharacter* Player;
	#pragma endregion

	#pragma region Private Attributes

	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	// Sets default values for this actor's properties
	ATradeSystem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Set NPC variable value.
	/// </summary>
	/// <param name="NewNPC">- New NPC value.</param>
	void SetNPC(class ABaseCharacter* NewNPC);

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Buy an item (for player).
		/// </summary>
		/// <param name="ItemIndex">- Inventory index.</param>
		/// <param name="ItemAmount">- Amount to buy.</param>
		void BuyItem(int ItemIndex, int ItemAmount);

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Sell an item (for player).
		/// </summary>
		/// <param name="ItemIndex">- Inventory index.</param>
		/// <param name="ItemAmount">- Amount to sell.</param>
		void SellItem(int ItemIndex, int ItemAmount);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Get item buy price.
		/// </summary>
		/// <param name="Item">- Item to get price.</param>
		/// <returns>Item buy price.</returns>
		int GetItemBuyPrice(TSubclassOf<class ABaseItem> Item);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Get item sell price.
		/// </summary>
		/// <param name="Item">- Item to get price.</param>
		/// <returns>Item sell price.</returns>
		int GetItemSellPrice(TSubclassOf<class ABaseItem> Item);
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
