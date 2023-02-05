#pragma once

#include <boost/asio/spawn.hpp>
#include <boost/beast.hpp>
#include <fmt/core.h>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;

// Report a failure
void fail(beast::error_code ec, char const *what);