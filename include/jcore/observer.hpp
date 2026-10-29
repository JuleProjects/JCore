#pragma once

/**
 * @brief   Observer pattern
 * @file    observer.hpp
 * @author  Praween Shinonawanik
 */

#include <span>

#include "delegate.hpp"

namespace jcore {

// template <typename... Args>
// class Subject;

template <typename... Args>
class Observer {
public:
  class Subject {
    using ObserverType = Observer<Args...>;

  public:
    void Add(ObserverType &target) {
      for (ObserverType *obs = head_; obs != nullptr; obs = obs->next_) {
        if (obs == &target) { return; }
      }
      target.next_ = head_;
      head_        = &target;
    }

    void Remove(ObserverType *target) {
      if (!target) { return; }

      for (ObserverType **obs = &head_; *obs != nullptr; obs = &(*obs)->next_) {
        if (*obs != target) { continue; }
        *obs          = target->next_;
        target->next_ = nullptr;
        return;
      }
    }

    void Notify(Args... args) {
      for (ObserverType *obs = head_; obs != nullptr; obs = obs->next_) {
        obs->on_notify(args...);
      }
    }

  protected:
    ObserverType *head_{};
  };

  Delegate<void(Args...)> on_notify;

private:
  Observer *next_{};
};

} // namespace jcore
