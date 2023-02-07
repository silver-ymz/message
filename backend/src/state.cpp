/**
 * @file state.cpp
 * @brief State class implementation.
 *
 * @author salvor
 * @version 0.1
 * @date 2023-02-05
 *
 * Copyright (c) 2023 Salvor
 */

#include "state.h"
#include "session.h"

void State::send_to_all(PassMsg msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto &session : sessions_) {
        session.session->send(msg);
    }
}

void State::join(SessionInfo session) {
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.insert(session);
}

void State::leave(SessionInfo session) {
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.erase(session);
}