// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Composite/USlimCompositeBase.h"
#include "USlimLeaf.generated.h"

/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API UUSlimLeaf : public UUSlimCompositeBase
{
	GENERATED_BODY()
	
public:
	virtual void ApplyFunction(FuncType Function) override;//覆写应用函数
};
