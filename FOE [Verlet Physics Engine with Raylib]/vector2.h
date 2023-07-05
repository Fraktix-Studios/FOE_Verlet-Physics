#pragma once

struct Vec2 {
	constexpr Vec2(float x = 0.0f, float y = 0.0f) noexcept : x(x), y(y) {}

	float x, y;

	constexpr const Vec2& operator-(const Vec2& other) const noexcept {
		return Vec2{ x - other.x, y - other.y };
	}
	constexpr const Vec2& operator+(const Vec2& other) const noexcept {
		return Vec2{ x + other.x, y + other.y };
	}
	constexpr const Vec2& operator/(const float factor) const noexcept {
		return Vec2{ x / factor, y / factor };
	}
	constexpr const Vec2& operator*(const float factor) const noexcept {
		return Vec2{ x * factor, y * factor };
	}
	constexpr const bool operator==(const Vec2& otherVector) const noexcept {
		return (x == otherVector.x && y == otherVector.y) == true;
	}
};

struct IntVec2 {
	constexpr IntVec2(int x = 0, int y = 0) noexcept : x(x), y(y) {}

	int x, y;

	constexpr const IntVec2& operator-(const IntVec2& other) const noexcept {
		return IntVec2{ x - other.x, y - other.y };
	}
	constexpr const IntVec2& operator+(const IntVec2& other) const noexcept {
		return IntVec2{ x + other.x, y + other.y };
	}
	constexpr const IntVec2& operator/(const int factor) const noexcept {
		return IntVec2{ x / factor, y / factor };
	}
	constexpr const IntVec2& operator*(const int factor) const noexcept {
		return IntVec2{ x * factor, y * factor };
	}
	constexpr const bool operator==(const IntVec2& otherVector) const noexcept {
		return (x == otherVector.x && y == otherVector.y) == true;
	}
};