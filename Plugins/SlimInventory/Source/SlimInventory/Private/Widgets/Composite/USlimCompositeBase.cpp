// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Composite/USlimCompositeBase.h"


void UUSlimCompositeBase::Collapse()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUSlimCompositeBase::Expand()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}