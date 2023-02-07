/**
 * @file session.h
 * @brief Session class definition. Session class is used to handle a single
 * connection.
 *
 * @author salvor
 * @version 0.1
 * @date 2023-02-05
 *
 * Copyright (c) 2023 Salvor
 */

#pragma once

#include "base.h"
#include "state.h"
#include "websocket.h"

#include <memory>
#include <queue>

/**
 * @brief Session class, handle a single connection.
 * @details Session class is used to handle a single connection. Session class
 * is a shared_ptr enabled class, so it can be shared between threads.
 * @see State
 */
class Session : public std::enable_shared_from_this<Session> {
  public:
    /**
     * @brief Construct a new Session object.
     * @details Construct a new Session object, and start handling the
     * connection.
     *
     * @param socket A socket object, which is used to communicate with the
     * client.
     * @param state A shared_ptr to a State object, which is used to store the
     * state of the server.
     */
    Session(tcp::socket &&socket, std::shared_ptr<State> state);
    /**
     * @brief Destroy the Session object.
     * @details Destroy the Session object, and close the connection.
     */
    ~Session();

    /**
     * @brief Run the session.
     * @details Run the session, start handling the connection.
     */
    void run();
    /**
     * @brief Send a message to the client.
     * @details Send a message to the client.
     *
     * @param msg The message to be sent.
     */
    void send(PassMsg msg);

  private:
    /**
     * @brief The websocket object.
     * @details The websocket object, which is used to communicate with the
     * client. The websocket object is built on top of the socket object. The
     * websocket object is used to handle the websocket protocol. It can keep
     * the connection alive, and send and receive messages.
     */
    WebSocket ws_;
    std::string username_;
    /**
     * @brief The buffer object.
     * @details The buffer object, which is used to store the data received from
     * the client.
     */
    std::shared_ptr<beast::flat_buffer> buffer_;
    /**
     * @brief The state object.
     * @details The state object, which is used to store the state of the
     * server.
     */
    std::shared_ptr<State> state_;
    /**
     * @brief The queue object.
     * @details The queue object, which is used to store the messages to be
     * sent to the client.
     */
    std::queue<std::string> queue_;

    /**
     * @brief Read a message from the client.
     * @details Read a message from the client.
     */
    void do_read();
    /**
     * @brief Dispatch a function to handle the message.
     * @details It can avoid the blocking of the main thread. It can also avoid
     * the blocking of the websocket object.
     */
    void on_run();
    /**
     * @brief Begin to accept a message from the client.
     * @details Begin to accept a message from the client. It is called by
     * on_run(). It can do some preparation work before accepting a message.
     * Meanwhile, it will call on_read() to handle the message.
     *
     * @param ec Error code.
     */
    void on_accept(beast::error_code ec);
    /**
     * @brief Handle the message.
     * @details Handle the message. It is called by on_accept(). It can do some
     * post-processing work after accepting a message. Meanwhile, it will call
     * on_write() to send a message to the client.
     *
     * @param ec Error code.
     * @param bytes_transferred The number of bytes transferred.
     */
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    /**
     * @brief Send a message to the client.
     * @details Send a message to the client. It is called by send().
     * It posts a function to the io_context, which is used to send a message
     * to the client. It can avoid the blocking of receiving multiple messages
     * at the same time.
     *
     * @param msg The message to be sent.
     */
    void on_send(const std::string msg);
    /**
     * @brief Begin to send a message to the client.
     * @details Begin to send a message to the client. It is called by
     * on_send() and self. It writes a message to the websocket object.
     * When the queue is not empty, it will call self to send the next message.
     *
     * @param ec Error code.
     */
    void on_write(beast::error_code ec, std::size_t bytes_transferred);
};