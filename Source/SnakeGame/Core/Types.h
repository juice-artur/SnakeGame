// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace SnakeGame
{
	struct Dimensions
	{
		int32 width;
		int32 height;
	};

	enum class CellyType
	{
		Empty,
		Wall,
		//Food
		//Snake
	};

	struct Settings
	{
		Dimensions gridSize;
	};
}