// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SlimInventoryPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API ASlimInventoryPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	//重载Beginplay
	virtual void BeginPlay() override;
};
