// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//引入自定义的枚举类型
#include "Types/InventoryGridTypes.h"
#include "Slim_InventoryGrid.generated.h"

class UInventoryGridSlot;
class UCanvasPanel;
class USlimInventoryComponent;

/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API USlim_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	//覆写初始化函数
	virtual void NativeOnInitialized() override;
	//获取小部件的类型
	EInventory_ItemCategory GetItemCategory() const { return ItemCatgory; }

	//定义添加小部件的函数
	UFUNCTION()
	void AddItem(USlimInventoryItem* Item);

private:
	//自定义初始化Grid函数
	void ConstructGrid();


	//声明小部件的类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	EInventory_ItemCategory ItemCatgory;

	//声明指向InventoryComponent的指针
	TWeakObjectPtr<USlimInventoryComponent> InventoryComponent;

	//声明相应需要绑定的参数
	UPROPERTY()
	TArray< TObjectPtr<UInventoryGridSlot> > GridSlots;

	UPROPERTY( EditAnywhere , Category = "Inventory" )
	TSubclassOf<UInventoryGridSlot> GridSlotClass;

	UPROPERTY( meta=(BindWidget) )
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY( EditAnywhere , Category="Inventory" )
	int32 Rows;
	UPROPERTY( EditAnywhere , Category="Inventory")
	int32 Columns;
	
	UPROPERTY( EditAnywhere , Category="Inventory")
	float TileSize;

	//定义是否匹配属性的方法
	bool MatchesCategory( const USlimInventoryItem* Item ) const;
};
