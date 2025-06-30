// Fill out your copyright notice in the Description page of Project Settings.


#include "SlimInventoryPlayerController.h"
//引入插件类
#include "SlimInventory.h"

void ASlimInventoryPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogSlimInventory, Log , TEXT("Beginplay for SlimInventoryPlayerController") );
}
