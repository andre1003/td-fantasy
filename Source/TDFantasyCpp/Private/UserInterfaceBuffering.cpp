// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "UserInterfaceBuffering.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Misc/OutputDeviceNull.h"
#pragma endregion


#pragma region Constructor
// Sets default values
AUserInterfaceBuffering::AUserInterfaceBuffering()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
#pragma endregion

#pragma region Events
#pragma region Begin Play
// Called when the game starts or when spawned
void AUserInterfaceBuffering::BeginPlay()
{
	Super::BeginPlay();

}
#pragma endregion

#pragma region Tick
// Called every frame
void AUserInterfaceBuffering::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Buffer level up UI
	Buffer(LevelUpUIClass, &LevelUpLength);

	// Buffer pick item UI
	Buffer(ItemsUIClass, &ItemsLength);

	// Buffer added money UI
	Buffer(MoneyUIClass, &MoneyLength);

	// Buffer added experience UI
	Buffer(XPUIClass, &XPLength);
}
#pragma endregion
#pragma endregion

#pragma region Methods
void AUserInterfaceBuffering::Buffer(TSubclassOf<class UUserWidget> WidgetReference, int* Length)
{
	// If widget reference is a null pointer, exit
	if(!WidgetReference)
	{
		return;
	}

	// Get widgets
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, WidgetReference);

	// If length is equal to widgets length, exit
	if(*Length == Widgets.Num())
	{
		return;
	}

	// If widgets length is equal to 1 or less than length
	if(Widgets.Num() == 1 || Widgets.Num() < *Length)
	{
		// Set length
		*Length = Widgets.Num();

		// If it is bigger than 0, call ShowUI
		if(*Length > 0)
		{
			FOutputDeviceNull OutputDevice;
			Widgets[0]->CallFunctionByNameWithArguments(TEXT("ShowUI"), OutputDevice, nullptr, true);
		}
	}

	// If not, just set length
	else
	{
		*Length = Widgets.Num();
	}
}
#pragma endregion
