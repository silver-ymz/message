#include "state.h"
#include "session.h"

void State::send_to_all(const std::string &msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto &session : sessions_) {
        session->send(msg);
    }
}

void State::join(Session *session) {
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.insert(session);
}

void State::leave(Session *session) {
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.erase(session);
}