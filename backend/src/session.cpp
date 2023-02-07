/**
 * @file session.cpp
 * @brief Session class implementation.
 *
 * @author salvor
 * @version 0.1
 * @date 2023-02-05
 *
 * Copyright (c) 2023 Salvor
 */

#include "session.h"
#include "message.h"
#include "state.h"
#include "websocket.h"

#include <fmt/format.h>
#include <memory>
#include <utility>

Session::Session(tcp::socket &&socket, std::shared_ptr<State> state)
    : ws_(std::move(socket)), state_(std::move(state)),
      buffer_(std::make_shared<beast::flat_buffer>()) {}

Session::~Session() {
    state_->leave({this->shared_from_this(), username_});
    state_->send_to_all(std::make_shared<Message>(fmt::format(
        R"({{"type": "user_left", "username": "{}"}})", username_)));
}

void Session::run() {
    asio::dispatch(
        ws_.get_executor(),
        beast::bind_front_handler(&Session::on_run, shared_from_this()));
}

void Session::on_run() {
    ws_.run();

    // Accept the websocket handshake
    ws_.async_accept(
        beast::bind_front_handler(&Session::on_accept, shared_from_this()));
}

void Session::on_accept(beast::error_code ec) {
    if (ec)
        return fail(ec, "accept");

    username_ = ws_.login();

    state_->send_to_all(std::make_shared<Message>(fmt::format(
        R"({{"type": "user_joined", "username": "{}"}})", username_)));

    state_->join({this->shared_from_this(), username_});

    ws_.login_success();

    // Read a message
    do_read();
}

void Session::do_read() {
    // Read a message into our buffer
    ws_.async_read(*buffer_, beast::bind_front_handler(&Session::on_read,
                                                       shared_from_this()));
}

void Session::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    // This indicates that the session was closed
    if (ec == websocket::error::closed || ec == asio::error::eof) {
        return;
    }

    if (ec) {
        return fail(ec, "read");
    }

    state_->send_to_all(
        std::make_shared<Message>(beast::buffers_to_string(buffer_->data())));
    buffer_->consume(buffer_->size());
    do_read();
}

void Session::send(PassMsg msg) {

    // Post our work to the strand, this ensures
    asio::post(ws_.get_executor(),
               beast::bind_front_handler(&Session::on_send, shared_from_this(),
                                         msg->stringify()));
}

void Session::on_send(const std::string msg) {
    queue_.push(msg);

    // Are we already writing?
    if (queue_.size() > 1) {
        return;
    }

    ws_.async_write(
        asio::buffer(queue_.front()),
        beast::bind_front_handler(&Session::on_write, shared_from_this()));
}

void Session::on_write(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec == websocket::error::closed || ec == asio::error::eof) {
        return;
    }

    if (ec) {
        return fail(ec, "write");
    }

    queue_.pop();

    if (!queue_.empty()) {
        ws_.async_write(
            asio::buffer(queue_.front()),
            beast::bind_front_handler(&Session::on_write, shared_from_this()));
    }
}