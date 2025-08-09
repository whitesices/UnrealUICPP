// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"//引入游戏Tag类
#include "Blueprint/UserWidget.h"
#include "SlimHoverItem.generated.h"

class USlimInventoryItem;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API USlimHoverItem : public UUserWidget
{
	GENERATED_BODY()

public:

	//获取对应的小部件类型
	FGameplayTag GetItemType() const;
	//设置图片
	void SetImageBrush(const FSlateBrush& Brush) const;
	//更新文本
	void UpdateStackCountText(const int32 Count ) ;

	//获取/修改私有成员变量
	int32 GetPreviousGridIndex() const { return PreviousGridIndex; }
	void SetPreviousGridIndex(int32 Index) { PreviousGridIndex = Index; }
	FIntPoint GetGridDimensions() const { return GridDimensions; }
	void SetGridDimensions(FIntPoint Diemensions) { GridDimensions = Diemensions; }
	USlimInventoryItem* GetInventoryItem() const;
	void SetInventoryItem(USlimInventoryItem* Item);
	bool IsStackable() const { return bIsStackable; }
	void SetIsStackable(bool bStack);
	int32 GetStackCount() const { return StackCount; }

private:
	UPROPERTY( meta=(BindWidget) )
	TObjectPtr<UImage> Image_Icon;
	
	UPROPERTY( meta=(BindWidget) )
	TObjectPtr<UTextBlock> Text_StackCount;

	//定义私有成员变量
	int32 PreviousGridIndex;
	FIntPoint GridDimensions;
	TWeakObjectPtr<USlimInventoryItem> InventoryItem;
	bool bIsStackable{false};
	int32 StackCount{0};
};
