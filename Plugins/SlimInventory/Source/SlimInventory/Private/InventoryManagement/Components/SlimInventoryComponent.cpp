// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/SlimInventoryComponent.h"
//引入自定义的UI
#include "Widgets/Inventory/InventoryBase/Slim_InventoryBase.h"

// Sets default values for this component's properties
USlimInventoryComponent::USlimInventoryComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void USlimInventoryComponent::ToggleInventoryUI()
{
	//通过定义的Flag来判断UI是否打开,执行bool值相反的操作
	if (bIsInventoryUIOpen)
	{
		CloseInventoryUI();
	}
	else
	{
		OpenInventoryUI();
	}
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
	//初始化关闭UI
	CloseInventoryUI();
}

void USlimInventoryComponent::OpenInventoryUI()
{
	//判断UI变量是否有效
	if (!IsValid(InventoryUIMenu))
	{
		UE_LOG( LogTemp , Warning , TEXT("Inventory is null") );
		return;
	}

	//设置UI可见性为可见
	InventoryUIMenu->SetVisibility( ESlateVisibility::Visible );
	//设置UI为打开状态
	bIsInventoryUIOpen = true;

	if (!OwningPlayerController.IsValid()) return;

	//设置输入模式
	FInputModeGameAndUI InputMode;
	OwningPlayerController->SetInputMode(InputMode);
	OwningPlayerController->SetShowMouseCursor(true);
}

void USlimInventoryComponent::CloseInventoryUI()
{
	//判断UI变量是否有效
	if (!IsValid(InventoryUIMenu))
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is null"));
		return;
	}
	//设置UI可见性为不可见
	InventoryUIMenu->SetVisibility( ESlateVisibility::Collapsed );
	//设置UI为关闭状态
	bIsInventoryUIOpen = false;

	if (!OwningPlayerController.IsValid()) return;

	//设置输入模式
	FInputModeGameAndUI InputMode;
	OwningPlayerController->SetInputMode(InputMode);
	OwningPlayerController->SetShowMouseCursor(false);
}

