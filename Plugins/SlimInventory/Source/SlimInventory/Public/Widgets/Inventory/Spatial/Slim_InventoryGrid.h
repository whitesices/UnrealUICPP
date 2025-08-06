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
class USlimInventoryItem;

class USlimInventoryItemComponent;
struct FSlimItemManifest;

struct FSlimImageFragment;
struct FSlimGridFragment;
class USlimSlottedItem;
class USlimHoverItem;
struct FGameplayTag;
//声明网格状态
enum class ESlimGridSlotState : uint8;

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
	//覆写Tick函数
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//获取小部件的类型
	EInventory_ItemCategory GetItemCategory() const { return ItemCatgory; }
	//获取小部件是否还有空间的结果函数、
	FSlimSlotAvailabilityResult HasRoomForItem( const USlimInventoryItemComponent* ItemComponent );

	//添加bItem索引
	/*void AddItemToIndices(const FSlimSlotAvailabilityResult& Result , USlimInventoryItem* NewItem );*/

	//定义添加小部件的函数
	UFUNCTION()
	void AddItem(USlimInventoryItem* Item);

	//定义更新堆叠数量的函数蓝图可调用
	UFUNCTION(Category="Inventory")
	void AddStackNumer(const FSlimSlotAvailabilityResult& Result);

	//定义SlottedItem的点击事件
	UFUNCTION()
	void OnSlottedItemClicked( int32 InGridIndex , const FPointerEvent& InMouseEvent );

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

	//声明SlotItem
	UPROPERTY( EditAnywhere , Category = "Inventory" )
	TSubclassOf<USlimSlottedItem> SlottedItemClass;

	//建立slottedItem的键值对映射
	UPROPERTY( VisibleAnywhere , Category = "Inventory")
	TMap< int32, TObjectPtr<USlimSlottedItem> > SlottedItems;

#pragma region 拖拽操作相关
	//声明HoverItem
	UPROPERTY(EditAnywhere , Category = "Inventory")
	TSubclassOf<USlimHoverItem> HoverItemClass;
	//声明HoverItem的变量
	UPROPERTY()
	TObjectPtr<USlimHoverItem> HoverItem;

	//声明定义图块参数以及前一个图块参数
	FSlimInventoryTileParameters TileParameters;
	FSlimInventoryTileParameters LastTileParameters;

	//声明拖拽的Index 和空间查询该位置可以放置的Item
	int32 ItemDropIndex{INDEX_NONE};
	FSlimSpaceQueryResult CurrentSpaceQueryResult;

	//添加标志位
	bool bMouseWithCanvas;
	bool bLastMouseWithCanvas;

	//声明变量记录上一个高亮的鼠标索引
	int32 LastHighlightedIndex;
	//声明变量记录上一个高亮的鼠标位置
	FIntPoint LastHighlightedPosition;
#pragma endregion

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
	//声明私有的获取结果都函数
	FSlimSlotAvailabilityResult HasRoomForItem(const USlimInventoryItem* Item);
	FSlimSlotAvailabilityResult HasRoomForItem(const FSlimItemManifest& Manifest);

	//添加bItem索引
	void AddItemToIndices(const FSlimSlotAvailabilityResult& Result, USlimInventoryItem* NewItem);
	//在索引里添加
	void AddItemAtIndex(USlimInventoryItem* Item , const int32 index , const bool bStackable , const int32 StackAmount );
	//绘制尺寸
	FVector2D GetDrawSize( const FSlimGridFragment* GridFragment ) const;
	//设置插槽小部件的图片
	void SetSlottedItemImage(  const USlimSlottedItem* SlottedItem , const FSlimGridFragment* GridFragment , const FSlimImageFragment* ImageFragment ) const;

	//创建插槽小组件
	USlimSlottedItem* CreateSlottedItem(  USlimInventoryItem* Item , const bool bStackable , const int32 StackAmount , const FSlimGridFragment* GridFragment , const FSlimImageFragment* ImageFragment , const int32 index );
	//声明创建将slottedItem添加到主画布的函数方法
	void AddSlottedItemToCanvas( const int32 Index , const FSlimGridFragment* GridFragment , USlimSlottedItem* SlottedItem) const;

	//更新GridSlots
	/*void UpdateGridSlots( USlimInventoryItem* NewItem , const int32 Index );*/
	void UpdateGridSlots(USlimInventoryItem* NewItem, const int32 Index , bool bStackableItem , const int32 StackAmount );

	//声明一个函数来判断索引存在判断
	bool IsIndexClaimed( const TSet<int32>& CheckedIndices , const int32 index) const;

	//声明一个函数根据索引判断是否还有空间
	/*bool HasRoomAtIndex( UInventoryGridSlot* GridSlot , const FIntPoint& Dimensions);*/
	bool HasRoomAtIndex(const UInventoryGridSlot* GridSlot, const FIntPoint& Dimensions , const TSet<int32>& CheckedIndices , TSet<int32>& OutTentativelyClaimed , const FGameplayTag& ItemType ,const int32 MaxStackSize);
	//声明一个函数获取小部件维度位置
	FIntPoint GetItemDimensions( const FSlimItemManifest& Manifest) const;

	//声明一个函数来检查插槽是否被约束
	/*bool CheckSlotConstraints( const UInventoryGridSlot* SubGridSlot) const;*/
	bool CheckSlotConstraints(const UInventoryGridSlot* GridSlot,const UInventoryGridSlot* SubGridSlot , const TSet<int32>& CheckedIndecies , TSet<int32>& OutTentativelyClaimed , const FGameplayTag& ItemType, const int32 MaxStackSize) const;
	//声明一个函数判断Item的有效性
	bool HasValidItem( const UInventoryGridSlot* GridSlot ) const;

	//声明一个函数判断左上角的插槽
	bool IsUpperLeftSlot( const UInventoryGridSlot* GridSlot , const UInventoryGridSlot* SubGridSlot ) const;

	//声明一个小部件类型匹配
	bool DoesItemTypeMatch( const USlimInventoryItem* SubItem , const FGameplayTag& ItemType ) const;

	//声明一个函数判断是否超出网格边界
	bool IsInGridBounds( const int32 StartIndex , const FIntPoint& ItemDimensions) const;

	//声明一个函数来确定插槽是否被填满
	int32 DetermineFilAmountForSlot(const bool bStackable , const int32 MaxStackSize , const int32 AmountToFill , const UInventoryGridSlot* GridSlot) const;
	//声明一个函数来获取堆叠数量
	int32 GetStackAmount( const UInventoryGridSlot* GridSlot) const;
#pragma region 鼠标相关处理函数
	//定义鼠标右击事件
	bool IsRightClick(const FPointerEvent& InMouseEvent) const;
	//定义鼠标左击事件
	bool IsLeftClick(const FPointerEvent& InMouseEvent) const;

	//声明Pickup函数
	void PickUp( USlimInventoryItem* ClickedInventoryItem , const int32 GridIndex);
	//声明绑定的HoverItem的函数
	void AssignHoverItem(USlimInventoryItem* InventoryItem);
	//声明更新堆叠数量的HoverItem的函数
	void AssignHoverItem( USlimInventoryItem* InventoryItem , const int32 GridIndex , const int32 PreviousGridIndex );
	//声明捡起物品后移除方格内显示的物品
	void RemoveItemFromGrid(USlimInventoryItem* InventoryItem , const int32 GridIndex);

	//定义更新图块参数函数
	void UpdateTileParameters( const FVector2D& CanavsPosition , const FVector2D& MousePosition);
	//定义封装一个函数来计算HoverItem的size
	FIntPoint CalculateHoverItemSize( const FVector2D& CanvasPosition , const FVector2D& MousePosition ) const;
	//定义一个函数来计算图块象限
	EInventoryTileQuadrant CalculateTileQuadrant( const FVector2D& CanvasPosition, const FVector2D& MousePosition ) const;
	//定义一个函数更新图块参数
	void OnTileParametersUpdated(const FSlimInventoryTileParameters& Parameters );
	//定义一个函数计算初始起点
	FIntPoint CalculateStartPoint( const FIntPoint& Coordinate,const FIntPoint& Dimensions , const EInventoryTileQuadrant Quadrant ) const ;
	//定义一个函数判断滑动的位置是否有效
	/*FSlimSpaceQueryResult CheckHoverPosition( const FIntPoint& Position , const FIntPoint& Dimensions) const;*/
	FSlimSpaceQueryResult CheckHoverPosition(const FIntPoint& Position, const FIntPoint& Dimensions);
	//定义一个函数来判断是否超出当前的画布边界
	bool CursorExitedCanvas(const FVector2D& BoundaryPos , const FVector2D& BoundarySize , const FVector2D& Location );

	//定义高亮和不高亮的方法
	void HighlightSlots( const int32 Index , const FIntPoint& Dimensions);
	void UnHighlightSlots( const int32 Index, const FIntPoint& Dimensions );
	//声明改变Hover状态的方法
	void ChangeHoverType( const int32 Index , const FIntPoint& Dimensions , ESlimGridSlotState GridSlotState );
#pragma endregion

#pragma region 鼠标滑动网格操作
	UFUNCTION()
	void OnSlottedClicked( int32 GridIndex , const FPointerEvent& MouseEvent );
	UFUNCTION()
	void OnSlottedHovered(int32 GridIndex, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnSlottedUnhovered(int32 GridIndex, const FPointerEvent& MouseEvent);

	void PutDown( const int32 Index );
	void ClearHoverItem();
#pragma endregion
};
