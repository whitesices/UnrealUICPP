// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/Inventory_HighLightStaticMesh.h"

void UInventory_HighLightStaticMesh::Highlight_Implementation()
{
	//判断材质是否有效
	SetOverlayMaterial(HighlightMaterial);
	/*else
	{
		UE_LOG(LogSlimInventory , Log , TEXT("HightlightMaterial is null "));
	}*/
}

void UInventory_HighLightStaticMesh::UnHightlight_Implementation()
{
	SetOverlayMaterial(nullptr);
}
