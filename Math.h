#pragma once
#include <cmath>
namespace Math {
	template<typename T>
	constexpr T PI = T(3.141592653589793238462643383279502884L);

	template<typename T>
	constexpr T TO_DEGREES = 180.0L / PI<T>;

	template<typename T>
	constexpr T TO_RADIANS = PI<T> / 180.0L;

	template<typename T>
	constexpr T radians(const T degrees) noexcept { return TO_RADIANS<T> * degrees; };
};