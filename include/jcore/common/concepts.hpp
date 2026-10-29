#pragma once

#include <type_traits>

namespace jcore {

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

} // namespace jcore
