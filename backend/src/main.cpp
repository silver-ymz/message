/**
 * @file main.cpp
 * @brief Main entry point for the backend server, start the listener and run
 * the io_context. The io_context will run in a loop, and handle all the
 * asynchronous operations. The io_context will run in multiple threads, and the
 * number of threads is specified by the command line arguments. The listener
 * will listen on a port and accept new connections. When a new connection is
 * accepted, a new session is created and run. The session will handle the
 * connection.
 *
 * @author salvor
 * @version 0.1
 * @date 2023-02-05
 *
 * Copyright (c) 2023 Salvor
 */

#include "listener.h"

#include <boost/asio/signal_set.hpp>
#include <cstdint>
#include <fmt/core.h>
#include <memory>
#include <thread>

/**
 * @brief Main entry point for the backend server.
 * @details Main entry point for the backend server, start the listener and run
 * the io_context. The io_context will run in a loop, and handle all the
 * asynchronous operations. The io_context will run in multiple threads, and the
 * number of threads is specified by the command line arguments. The listener
 * will listen on a port and accept new connections. When a new connection is
 * accepted, a new session is created and run. The session will handle the
 * connection.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return int The exit code.
 */
int main(int argc, char **argv) {
    // Check command line arguments.
    if (argc != 4) {
        fmt::print(stderr, "Usage: {} <address> <port> <threads>", argv[0]);
        return EXIT_FAILURE;
    }
    auto const address = asio::ip::make_address(argv[1]);
    auto const port = static_cast<std::uint16_t>(std::atoi(argv[2]));
    auto const threads = std::max<int>(1, std::atoi(argv[3]));

    asio::io_context ioc;

    // Create and launch a listening port
    std::make_shared<Listener>(tcp::acceptor(ioc, {address, port}))->run();

    // Capture SIGINT and SIGTERM to perform a clean shutdown
    asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&ioc](boost::system::error_code const &, int) {
        // Stop the io_context. This will cause run() to return immediately,
        ioc.stop();
    });

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for (auto i = threads - 1; i > 0; --i) {
        v.emplace_back([&ioc] { ioc.run(); });
    }
    ioc.run();

    return EXIT_SUCCESS;
}