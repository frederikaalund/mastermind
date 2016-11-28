#pragma once
#include <array>
#include <cstdint>
#include <random>
#include <boost/container/static_vector.hpp>

enum class color: std::uint_fast8_t {
	// Primary colors
	black,
	red,
	green,
	yellow,
	blue,
	purple,
	cyan,
	white,
	// Ancilliary colors
	grey1 = 239,
	grey2 = 242,
	grey3 = 245,
	custom1 = 68,
};

const std::underlying_type_t<color> primary_colors_size{8};

template<typename T>
constexpr T decrement_with_rollover(T value, T max) {
	return (value - 1 + max) % max;
}

template<typename T>
constexpr T increment_with_rollover(T value, T max) {
	return (value + 1) % max;
}

template<typename T>
constexpr auto to_integral(T value) {
	return static_cast<std::underlying_type_t<T>>(value);
}

inline auto prev(color c) {
	return static_cast<color>(decrement_with_rollover(to_integral(c), primary_colors_size));
}

inline auto next(color c) {
	return static_cast<color>(increment_with_rollover(to_integral(c), primary_colors_size));
}

inline auto& operator++(color& c) {
	return c = next(c);
}

inline auto& operator--(color& c) {
	return c = prev(c);
}

const std::size_t max_pegs{primary_colors_size};

using code_pegs = boost::container::static_vector<color, max_pegs>;

namespace code_pegs_presets {

inline auto all_black(typename code_pegs::size_type size = max_pegs) {
	return code_pegs(size, color::black);
}

inline auto unique_sequence(typename code_pegs::size_type size = max_pegs) {
	code_pegs result(size);
	std::iota(result.begin(), result.end(), color::black);
	return result;
}

inline auto unique_shuffle(typename code_pegs::size_type size = max_pegs) {
	auto result = unique_sequence(max_pegs);
	std::random_device rd;
	std::mt19937 mt(rd());
	std::shuffle(result.begin(), result.end(), mt);
	result.resize(size);
	return result;
}

} // namespace code_pegs_presets
