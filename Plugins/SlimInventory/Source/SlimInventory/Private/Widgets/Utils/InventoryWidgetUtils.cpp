// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Utils/InventoryWidgetUtils.h"
//#include "InventoryWidgetUtils.h"
#include "Components/Widget.h"
#include "Blueprint/SlateBlueprintLibrary.h"

int32 UInventoryWidgetUtils::GetInventoryIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}

FIntPoint UInventoryWidgetUtils::GetPositionFromIndex(const int32 Index, const int32 Columns)
{
	return FIntPoint( Index % Columns , Index / Columns );
}

FVector2D UInventoryWidgetUtils::GetWidgetPosition(UWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	FVector2D PixelPosition;
	FVector2D ViewportPosition;

	USlateBlueprintLibrary::LocalToViewport( Widget , Geometry , USlateBlueprintLibrary::GetLocalTopLeft(Geometry) , PixelPosition , ViewportPosition );

	return ViewportPosition;
}
