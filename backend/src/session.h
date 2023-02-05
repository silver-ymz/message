#pragma once

#include "base.h"
#include "state.h"
#include <memory>
#include <queue>

class Session : public std::enable_shared_from_this<Session> {
  public:
    Session(tcp::socket &&socket, std::shared_ptr<State> state);
    ~Session();

    void run();
    void send(std::string msg);

  private:
    websocket::stream<beast::tcp_stream> ws_;
    std::shared_ptr<beast::flat_buffer> buffer_;
    std::shared_ptr<State> state_;
    std::queue<std::string> queue_;

    void do_read();
    void on_run();
    void on_accept(beast::error_code ec);
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void on_write(beast::error_code ec, std::size_t bytes_transferred);
    void on_send(const std::string &msg);
};