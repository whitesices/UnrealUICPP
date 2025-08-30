// Fill out your copyright notice in the Description page of Project Settings.


#include "Fragment/SlimItemFragment.h"

void FSlimHealthFragment::OnConsumeable(APlayerController* PC)
{
	//Get a stats component from the player controller or the PlayerController->GetPawn()
	//or get the ability system component and apply a GameplayEffect to restore health
	//or call an interface function for Healthing()

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage( -1 , 7.f , FColor::Red , FString::Printf(TEXT("Health Potion consumed! Healing by: %f") , HealthAmount) );
	}
}

void FSlimManaFragment::OnConsumeable(APlayerController* PC)
{
	//Restore Mana to the Playre like Health

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Blue, FString::Printf(TEXT("Health Potion consumed! Healing by: %f"), ManaAmount));
	}
}
