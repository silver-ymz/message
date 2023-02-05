#pragma once

#include "base.h"
#include "state.h"
#include <memory>

class Listener : public std::enable_shared_from_this<Listener> {
  public:
    explicit Listener(tcp::acceptor &&acceptor)
        : acceptor_(std::move(acceptor)), state_(std::make_shared<State>()){};

    void run();

  private:
    void on_accept(boost::system::error_code ec, tcp::socket socket);

    tcp::acceptor acceptor_;
    std::shared_ptr<State> state_;
};