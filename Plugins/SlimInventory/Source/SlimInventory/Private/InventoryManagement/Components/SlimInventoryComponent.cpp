// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/SlimInventoryComponent.h"
//引入自定义的UI
#include "Widgets/Inventory/InventoryBase/Slim_InventoryBase.h"

// Sets default values for this component's properties
USlimInventoryComponent::USlimInventoryComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}


void USlimInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructInventory();
	
}

void USlimInventoryComponent::ConstructInventory()
{
	OwningPlayerController = Cast<APlayerController>( GetOwner() );
	checkf( OwningPlayerController.IsValid() , TEXT("Inventory Component should have a Player Controller as Owner"));//检查PlayerController是否有效

	if ( !OwningPlayerController->IsLocalController() )
	{
		return;
	}

	InventoryUIMenu = CreateWidget<USlim_InventoryBase>( OwningPlayerController.Get() , InventoryUIMenuClass );
	InventoryUIMenu->AddToViewport();
}

