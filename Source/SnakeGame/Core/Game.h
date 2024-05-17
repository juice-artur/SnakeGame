// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame {
	class Grid;

	class Game
	{
	public:
		Game(const Settings& settings );
	private:
		const Settings c_settings;
		TSharedPtr<Grid> m_grid;
	};
}