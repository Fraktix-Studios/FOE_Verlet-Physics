#pragma once

#include <raylib.h>
#include <vector>
#include "vector2.h"

namespace foe
{
	// Application Settings
	constexpr int windowWidth = 1280;
	constexpr int windowHeight = 720;
	constexpr int windowFPS = 200;
	float applicationTime;


	// Verlet (Physics) Settings

	// Solver Settings
	constexpr int subSteps = 8;
	float stepDeltaTime = 0;
	float stepDeltaTimeTarget = 0;
	int frameRate = 0;

	// Object Settings
	constexpr Vec2 gravity = {0.0f, 100000.0f};
	constexpr IntVec2 startPosition = {windowWidth / 16 * 15, windowHeight / 4};
	constexpr int objectAmount = 8000;
	constexpr int objectGroupAmount = 4;
	constexpr float spawnDelay = 0.01f;
	float spawnTime = 0;
	constexpr float objectSpeed = 20000.0f;
	constexpr float objectBounciness = 0.0f;
	constexpr int minRadius = 4;
	constexpr int maxRadius = 4;
	constexpr float spawnAngleSpeed = 0.0f;
	constexpr float spawnAngleOffset = 1.0f;
	constexpr float spawnAngleMax = 0.5f;


	// Colors
	static Color getRainbow(float t)
	{
		const float r = sin(t);
		const float g = sin(t + 0.33f * 2.0f * PI);
		const float b = sin(t + 0.66f * 2.0f * PI);
		return {
			static_cast<uint8_t>(255.0f * r * r),
			static_cast<uint8_t>(255.0f * g * g),
			static_cast<uint8_t>(255.0f * b * b), 255
		};
	}

	static Color getRandColor(Color first, Color second)
	{
		const auto maxR = std::max(first.r, second.r);
		const auto maxG = std::max(first.g, second.g);
		const auto maxB = std::max(first.b, second.b);

		const auto minR = std::min(first.r, second.r);
		const auto minG = std::min(first.g, second.g);
		const auto minB = std::min(first.b, second.b);

		const int r = GetRandomValue(minR, maxR);
		const int g = GetRandomValue(minG, maxG);
		const int b = GetRandomValue(minB, maxB);

		return {
			static_cast<uint8_t>(r),
			static_cast<uint8_t>(g),
			static_cast<uint8_t>(b), 255
		};
	}

	inline int ClampInt(int max, int min, int input)
	{
		if (input <= min)
		{
			return min;
		}
		if (input >= max)
		{
			return max;
		}
		if (input <= max || input >= min)
		{
			return input;
		}
		return 0;
	}

	inline IntVec2 CalculateCellPosition(Vec2 position, int columns, int rows, int width, int height)
	{
		return {
			ClampInt(columns - 1, -1, static_cast<int>(position.x / static_cast<float>(width))),
			ClampInt(rows - 1, -1, static_cast<int>(position.y / static_cast<float>(height)))
		};
	}
}
