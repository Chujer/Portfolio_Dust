// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/WeaponDataAsset.h"

#include "CLog.h"
#include "Net/UnrealNetwork.h"

void UWeaponDataAsset::BeginDestroy()
{
	Super::BeginDestroy();

	CLog::Print("WeaponDataRemove");
}
