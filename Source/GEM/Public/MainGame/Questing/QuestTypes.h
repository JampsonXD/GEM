// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTypes.generated.h"

/**
 * 
 */

UENUM()
enum class QuestStatus : uint8
{
	Locked,
	InProgress,
	Completed
};

UENUM()
enum class TaskStatus : uint8
{
	Blocked,
	InProgress,
	Completed
};