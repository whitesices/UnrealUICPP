// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Composite/USlimLeaf.h"

void UUSlimLeaf::ApplyFunction(FuncType Function)
{
	Function(this);
}