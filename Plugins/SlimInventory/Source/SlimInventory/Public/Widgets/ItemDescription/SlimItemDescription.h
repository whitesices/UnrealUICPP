// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlimItemDescription.generated.h"

class USizeBox;

/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API USlimItemDescription : public UUserWidget
{
	GENERATED_BODY()
public:
	FVector2D GetBoxSize() const;
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox;
};
