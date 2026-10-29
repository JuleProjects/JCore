#pragma once

/**
 * @brief   Delegate
 * @file    Delegate.hpp
 * @author  Praween Shinonawanik
 */

#include <cassert>
#include <utility>

namespace jcore {

template <typename>
class Delegate;

template <typename R, typename... Args>
class Delegate<R(Args...)> {
public:
  /**
   * @brief Binds free function
   * @tparam func    function pointer
   */
  template <auto func>
  void Bind() noexcept {
    func_ = [](const void *, Args... args) -> R {
      return func(std::forward<Args>(args)...);
    };
  }

  /**
   * @brief Binds member function
   * @tparam C       class
   * @tparam func    member function pointer
   * @param[in]   inst    object pointer
   */
  template <auto func, class C>
  void Bind(C *inst) noexcept {
    inst_ = inst;
    func_ = [](const void *inst, Args... args) -> R {
      return (static_cast<C *>(const_cast<void *>(inst))->*func)(
          std::forward<Args>(args)...);
    };
  }

  template <auto func, class C>
  void Bind(const C *inst) noexcept {
    inst_ = inst;
    func_ = [](const void *inst, Args... args) -> R {
      return (static_cast<const C *>(inst)->*func)(std::forward<Args>(args)...);
    };
  }

  //! @brief Unbinds the delegate
  void Unbind() noexcept { func_ = nullptr; }

  //! @brief Calls delegate function
  R operator()(Args... args) const {
    assert(func_);
    return func_(inst_, std::forward<Args>(args)...);
  }

  //! @brief Checks if the delegate is binded
  explicit operator bool() const noexcept { return func_ != nullptr; }

private:
  R (*func_)(const void *, Args...){}; //!< function pointer
  const void *inst_{};                 //!< object instance
};

} // namespace jcore
