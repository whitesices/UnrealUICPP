// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlimInventory.h"
#include "Components/StaticMeshComponent.h"
//引入自定义的接口函数
#include "Interaction/Inventory_Hightlightable.h"
#include "Inventory_HighLightStaticMesh.generated.h"

/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API UInventory_HighLightStaticMesh : public UStaticMeshComponent , public IInventory_Hightlightable
{
	GENERATED_BODY()
public:
	//覆写实现接口函数
	//~Begin IInventory_Hightlightable Interface
	virtual void Highlight_Implementation() override;
	virtual void UnHightlight_Implementation() override;
	//~End IInventory_Hightlightable Interface

private:
	//声明高亮材质
	UPROPERTY( EditAnywhere , Category = "Inventory" )
	TObjectPtr<UMaterialInterface> HighlightMaterial;
	
};
