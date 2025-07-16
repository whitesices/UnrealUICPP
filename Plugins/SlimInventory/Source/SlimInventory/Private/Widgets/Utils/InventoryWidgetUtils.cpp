// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Utils/InventoryWidgetUtils.h"
//#include "InventoryWidgetUtils.h"

int32 UInventoryWidgetUtils::GetInventoryIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}

FIntPoint UInventoryWidgetUtils::GetPositionFromIndex(const int32 Index, const int32 Columns)
{
	return FIntPoint( Index % Columns , Index / Columns );
}
