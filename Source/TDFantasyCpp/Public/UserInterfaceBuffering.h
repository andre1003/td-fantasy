// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.

#pragma once

#pragma region Includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UserInterfaceBuffering.generated.h"
#pragma endregion


UCLASS()
class TDFANTASYCPP_API AUserInterfaceBuffering : public AActor
{
	GENERATED_BODY()
	

#pragma region Attributes
	#pragma region Public Attributes

	#pragma endregion

	#pragma region Protected Attributes
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WidgetReferences")
		TSubclassOf<class UUserWidget> LevelUpUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WidgetReferences")
		TSubclassOf<class UUserWidget> ItemsUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WidgetReferences")
		TSubclassOf<class UUserWidget> MoneyUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WidgetReferences")
		TSubclassOf<class UUserWidget> XPUIClass;
	#pragma endregion

	#pragma region Private Attributes
	int LevelUpLength;
	int ItemsLength;
	int MoneyLength;
	int XPLength;
	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	// Sets default values for this actor's properties
	AUserInterfaceBuffering();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	#pragma endregion

	#pragma region Protected Methods
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	#pragma endregion

	#pragma region Private Methods
private:
	/// <summary>
	/// Buffer a given widget refence.
	/// </summary>
	/// <param name="WidgetReference">- Widget reference.</param>
	/// <param name="Length">- Widget auxiliar length variable.</param>
	void Buffer(TSubclassOf<class UUserWidget> WidgetReference, int* Length);
	#pragma endregion
#pragma endregion





};
