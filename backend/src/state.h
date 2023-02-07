/**
 * @file state.h
 * @brief State class definition. State class is used to store the state of the
 * server. It maintains a set of sessions, and provides methods to send messages
 * to all sessions.
 *
 * @author salvor
 * @version 0.1
 * @date 2023-02-05
 *
 * Copyright (c) 2023 Salvor
 */

#pragma once

#include <memory>
#include <mutex>
#include <unordered_set>

class Session;
class Message;

using PassMsg = const std::shared_ptr<const Message>;

struct SessionInfo {
    std::shared_ptr<Session> session;
    std::string username;

    bool operator==(const SessionInfo &other) const {
        return session == other.session;
    }

    struct HashFunction {
        size_t operator()(const SessionInfo &self) const {
            return std::hash<std::shared_ptr<Session>>()(self.session);
        }
    };
};

/**
 * @brief State class, store the state of the server.
 * @details State class is used to store the state of the server. It maintains a
 * set of sessions, and provides methods to send messages to all sessions.
 * @see Session
 */
class State : std::enable_shared_from_this<State> {
  public:
    /**
     * @brief Construct a new State object.
     * @details Construct a new State object.
     */
    State() = default;

    /**
     * @brief Send a message to all sessions.
     * @details Send a message to all sessions. This method is thread-safe.
     * @see Session::send
     *
     * @param msg The message to be sent.
     */
    void send_to_all(PassMsg msg);
    /**
     * @brief Add a session to the state.
     * @details Add a session to the state. This method is thread-safe. The
     * session will be removed from the state when it is destroyed.
     * @see Session
     *
     * @param session A pointer to the session to be added.
     */
    void join(SessionInfo session);
    /**
     * @brief Remove a session from the state.
     * @details Remove a session from the state. This method is thread-safe. The
     * session will be removed from the state when it is destroyed.
     * @see Session
     *
     * @param session A pointer to the session to be removed.
     */
    void leave(SessionInfo session);

  private:
    /**
     * @brief The set of sessions.
     * @details The set of sessions. This set is thread-safe. The session will
     * be removed from the set when it is destroyed.
     */
    std::unordered_set<SessionInfo, SessionInfo::HashFunction> sessions_;
    /**
     * @brief The mutex used to protect the set of sessions.
     * @details The mutex used to protect the set of sessions. This mutex is
     * used to ensure thread-safety.
     */
    std::mutex mutex_;
};