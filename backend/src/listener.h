/**
 * @file listener.h
 * @brief Listener class definition, listen on a port and accept new
 * connections.
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
#include <memory>

/**
 * @brief Listener class, listen on a port and accept new connections.
 * @details When a new connection is accepted, a new session is created and run.
 * Listener class is a shared_ptr enabled class, so it can be shared between
 * threads.
 * @see Session
 * @see State
 */
class Listener : public std::enable_shared_from_this<Listener> {
  public:
    /**
     * @brief Construct a new Listener object.
     * @details Construct a new Listener object, and start listening on the
     * specified port.
     *
     * @param acceptor An acceptor object, which is used to listen on a port.
     * When main function build a tcp acceptor, then pass it to build an
     * acceptor object.
     */
    explicit Listener(tcp::acceptor &&acceptor)
        : acceptor_(std::move(acceptor)), state_(std::make_shared<State>()){};

    /**
     * @brief Run the listener.
     * @details Run the listener, start listening on the port.
     */
    void run();

  private:
    /**
     * @brief Accept a new connection.
     * @details Accept a new connection, and create a new session to handle it.
     *
     * @param ec Error code.
     * @param socket A socket object, which is used to communicate with the
     * client.
     */
    void on_accept(boost::system::error_code ec, tcp::socket socket);

    /**
     * @brief The acceptor object.
     * @details The acceptor object, which is used to listen on a port.
     */
    tcp::acceptor acceptor_;
    /**
     * @brief The state object.
     * @details The state object, which is used to store the state of the
     * application.
     */
    std::shared_ptr<State> state_;
};