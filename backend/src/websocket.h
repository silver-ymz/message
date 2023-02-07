/**
 * @file websocket.h
 * @brief WebSocket class
 *
 * @author salvor
 * @version 0.1
 * @date 2023-02-07
 *
 * Copyright (c) 2023 Salvor
 */

#pragma once

#include "base.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

/**
 * @brief WebSocket class
 * @details This class is a wrapper of boost::beast::websocket::stream
 * @see
 * https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/beast/ref/boost__beast__websocket__stream.html
 */
class WebSocket : public websocket::stream<beast::tcp_stream> {
  public:
    /**
     * @brief Construct a new WebSocket object
     *
     * @param socket Socket object.
     */
    explicit WebSocket(tcp::socket &&socket)
        : websocket::stream<beast::tcp_stream>(std::move(socket)) {}
    /**
     * @brief Run WebSocket
     * @details Set suggested timeout settings for the websocket. Set a
     * decorator to change the Server of the handshake. Set text mode.
     */
    void run();
    /**
     * @brief Login
     * @details Read message from client. If message is login info, return
     * username. If message is not login info, read again.
     *
     * @return std::string Username.
     */
    std::string login();
    /**
     * @brief Login success
     * @details Send login success message to client.
     */
    void login_success();
};