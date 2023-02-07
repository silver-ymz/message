/**
 * @file websocket.cpp
 * @brief WebSocket class implementation
 *
 * @author salvor
 * @version 0.1
 * @date 2023-02-07
 *
 * Copyright (c) 2023 Salvor
 */

#include "websocket.h"
#include "base.h"
#include "message.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <memory>

void WebSocket::run() {
    // Set suggested timeout settings for the websocket
    this->set_option(
        websocket::stream_base::timeout::suggested(beast::role_type::server));

    // Set a decorator to change the Server of the handshake
    this->set_option(
        websocket::stream_base::decorator([](websocket::response_type &res) {
            res.set(http::field::server,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                        " message-server-async");
        }));

    this->text(true);
}

std::string WebSocket::login() {
    for (;;) {
        beast::flat_buffer buffer;
        this->read(buffer);
        Message message(beast::buffers_to_string(buffer.data()));
        if (message.is_login_info()) {
            return message.get_username();
        }
    }
}

void WebSocket::login_success() {
    boost::beast::error_code ec;
    this->write(
        asio::buffer(std::string(R"({"type": "login", "success": true})")), ec);

    if (ec) {
        fail(ec, "write");
    }
}