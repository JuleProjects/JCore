#pragma once

/**
 * @brief   Event queue
 * @file    EvQueue.hpp
 * @author  Praween Shinonawanik
 */

#include <variant>

#include <boost/circular_buffer.hpp>

#ifdef WITH_JCORE_TS_EVENT_QUEUE
#include <readerwriterqueue.h>
#endif

namespace jcore {

//! Static interface
template <typename Derived>
class IEvQueue {
  friend Derived;

  IEvQueue() = default;

public:
  template <typename Event>
  void Push(Event &&ev) noexcept {
    derived().Push(std::forward<Event>(ev));
  }

  template <class Handler, typename... Args>
  void Dispatch(Handler &handle, Args &&...args) noexcept {
    derived().Dispatch(handle, std::forward<Args>(args)...);
  }

  std::size_t Size() const noexcept { return derived().Size(); }

  std::size_t Capacity() const noexcept { return derived().Capacity(); }

private:
  Derived &derived() noexcept { return static_cast<Derived &>(*this); }
  Derived const &derived() const noexcept {
    return static_cast<Derived const &>(*this);
  }
};

//! Event queue
template <typename... Events>
class EvQueue : public IEvQueue<EvQueue<Events...>> {
  using Queue = boost::circular_buffer<std::variant<Events...>>;

public:
  EvQueue() = default;

  explicit EvQueue(std::size_t capacity) noexcept : q_{capacity} {}

  template <typename Event>
  void Push(Event &&ev) noexcept {
    q_.push_back(std::forward<Event>(ev));
  }

  template <class Handler, typename... Args>
  void Dispatch(Handler &handler, Args... args) noexcept {
    while (q_.size()) {
      const auto ev = q_.front();
      q_.pop_front();
      std::visit([&](auto &&ev_) { handler.OnEvent(args..., ev_); }, ev);
    }
  }

  void Clear() noexcept { q_.clear(); }

  std::size_t Capacity() const noexcept { return q_.capacity(); }

  std::size_t Size() const noexcept { return q_.size(); }

private:
  Queue q_;
};

template <typename... Events>
struct EvQueue<std::variant<Events...>> : EvQueue<Events...> {
  using EvQueue<Events...>::EvQueue;
};

#ifdef WITH_JCORE_TS_EVENT_QUEUE

//! Thread-safe event queue
template <typename... Events>
class TsEvQueue : public IEvQueue<TsEvQueue<Events...>> {
  using Queue = moodycamel::ReaderWriterQueue<std::variant<Events...>>;

public:
  TsEvQueue() = default;

  TsEvQueue(std::size_t capacity) noexcept : q_{capacity} {}

  template <typename Event>
  void Push(Event &&ev) noexcept {
    q_.try_emplace(std::forward<Event>(ev));
  }

  template <class Handler, typename... Args>
  void Dispatch(Handler &handler, Args &&...args) noexcept {
    std::variant<Events...> ev;
    while (q_.try_dequeue(ev)) {
      std::visit([&](auto &&ev_) { handler.OnEvent(args..., ev_); }, ev);
    }
  }

  void Clear() noexcept {
    std::variant<Events...> ev;
    while (q_.try_dequeue(ev));
  }

  std::size_t Capacity() const noexcept { return q_.max_capacity(); }

  std::size_t Size() const noexcept { return q_.size_approx(); }

private:
  Queue q_;
};

template <typename... Events>
struct TsEvQueue<std::variant<Events...>> : TsEvQueue<Events...> {
  using TsEvQueue<Events...>::TsEvQueue;
};

#endif // WITH_JCORE_TS_EVENT_QUEUE

} // namespace jcore
