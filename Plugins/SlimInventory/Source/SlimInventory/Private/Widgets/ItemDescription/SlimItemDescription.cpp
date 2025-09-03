// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ItemDescription/SlimItemDescription.h"
#include "Components/SizeBox.h"

FVector2D USlimItemDescription::GetBoxSize() const
{
	return SizeBox->GetDesiredSize();
}