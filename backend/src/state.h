#pragma once

#include <memory>
#include <mutex>
#include <unordered_set>

class Session;

class State : std::enable_shared_from_this<State> {
  public:
    State() = default;

    void send_to_all(const std::string &msg);
    void join(Session *session);
    void leave(Session *session);

  private:
    std::unordered_set<Session *> sessions_;
    std::mutex mutex_;
};