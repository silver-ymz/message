/**
 * @file base.h
 * @brief Base header file, include all necessary headers and define some common
 * functions.
 *
 * @author salvor
 * @version 0.1
 * @date 2023-02-05
 *
 * Copyright (c) 2023 Salvor
 */

#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>
#include <fmt/core.h>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;

/**
 * @brief Failure handle function. Print error message to stderr.
 *
 * @param ec Error code.
 * @param what Error message.
 */
void fail(beast::error_code ec, char const *what);