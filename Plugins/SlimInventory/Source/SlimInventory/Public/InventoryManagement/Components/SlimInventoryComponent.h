// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//引入FastArray
#include "InventoryManagement/FastArray/InventoryFastArray.h"
#include "SlimInventoryComponent.generated.h"

class USlim_InventoryBase;
class USlimInventoryItem;
class USlimInventoryItemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, USlimInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChange , const FSlimSlotAvailabilityResult& , Result);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) , Blueprintable )
class SLIMINVENTORY_API USlimInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USlimInventoryComponent();

	//声明触发打开InventoryUI的函数
	void ToggleInventoryUI();

	//声明添加Item的函数
	UFUNCTION( BlueprintCallable , BlueprintAuthorityOnly , Category="Inventory")
	void TryAddItem( USlimInventoryItemComponent* ItemComponent);

	//声明创建网络同步函数 ， 即从都从服务器发送 ， 客户端去接收
	UFUNCTION( Server , Reliable )
	void Server_AddNewItem( USlimInventoryItemComponent* ItemComponent , int32 StackCount );
	UFUNCTION( Server , Reliable )
	void Server_AddStacksToItem( USlimInventoryItemComponent* ItemComponent , int32 StackCount , int32 Remainder);

	//复写获取生命周期内网络复制函数
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//声明添加复制子对象
	void AddRepSubObject(UObject* Subobject);

	//声明委托多播变量
	FInventoryItemChange OnItemAdded;
	FInventoryItemChange OnItemRemoved;
	FNoRoomInInventory OnNoRoomInInventory;

	FStackChange OnStackChange;

protected:
	virtual void BeginPlay() override;

private:	
	//声明PlayerController变量
	TWeakObjectPtr<APlayerController> OwningPlayerController;

	//建立新的初始化函数
	void ConstructInventory();

	//声明自定义的UI基类变量
	UPROPERTY()
	TObjectPtr<USlim_InventoryBase> InventoryUIMenu;

	//声明一个变量来存储UI的类
	UPROPERTY( EditAnywhere , Category = "Inventory" )
	TSubclassOf<USlim_InventoryBase> InventoryUIMenuClass;

	//声明一个FastArray来存储条目
	UPROPERTY(Replicated)
	FSlimInventoryFastArray InventoryList;

	//声明flag来存储UI是否打开
	bool bIsInventoryUIOpen;
	//声明打开UI的函数
	void OpenInventoryUI();
	//声明关闭UI的函数
	void CloseInventoryUI();
		
};
