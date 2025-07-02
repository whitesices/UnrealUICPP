// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGridSlot.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API UInventoryGridSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	//设置小格子索引
	void SetTileIndex(int32 Index) { TileIndex = Index; }
	//获取TileIndex
	int32 GetTileIndex() { return TileIndex; }
	
private:
	int32 TileIndex;

	UPROPERTY( meta=(BindWidget) )
	TObjectPtr<UImage> Image_GridSlot;
};
