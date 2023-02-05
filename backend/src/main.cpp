#include "listener.h"
#include <cstdint>
#include <fmt/core.h>
#include <memory>
#include <thread>

int main(int argc, char *argv[]) {
    // Check command line arguments.
    if (argc != 4) {
        fmt::print(stderr, "Usage: {} <address> <port> <threads>", argv[0]);
        return EXIT_FAILURE;
    }
    auto const address = asio::ip::make_address(argv[1]);
    auto const port = static_cast<std::uint16_t>(std::atoi(argv[2]));
    auto const threads = std::max<int>(1, std::atoi(argv[3]));

    asio::io_context ioc;

    std::make_shared<Listener>(tcp::acceptor(ioc, {address, port}))->run();

    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for (auto i = threads - 1; i > 0; --i) {
        v.emplace_back([&ioc] { ioc.run(); });
    }
    ioc.run();

    return EXIT_SUCCESS;
}