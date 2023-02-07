/**
 * @file listener.cpp
 * @brief Listener class implementation
 *
 * @author salvor
 * @version 0.1
 * @date 2023-02-05
 *
 * Copyright (c) 2023 Salvor
 */

#include "listener.h"
#include "base.h"
#include "session.h"

void Listener::run() {
    acceptor_.async_accept(
        [self = shared_from_this()](boost::system::error_code ec,
                                    tcp::socket socket) {
            self->on_accept(ec, std::move(socket));
        });
}

void Listener::on_accept(boost::system::error_code ec, tcp::socket socket) {
    if (ec) {
        fail(ec, "accept");
    } else {
        std::make_shared<Session>(std::move(socket), state_)->run();
    }
    run();
}