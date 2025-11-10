// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Composite/USlimComposite.h"
#include "Blueprint/WidgetTree.h"

void UUSlimComposite::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		if (UUSlimCompositeBase* Composite = Cast<UUSlimCompositeBase>(Widget); IsValid(Composite))
		{
			Children.Add(Composite);
			Composite->Collapse();
		}
	});
}

void UUSlimComposite::ApplyFunction(FuncType Function)
{
	for ( auto& Child:Children )
	{
		Child->ApplyFunction(Function);
	}
}

void UUSlimComposite::Collapse()
{
	for ( auto& Child:Children )
	{
		Child->Collapse();
	}
}
