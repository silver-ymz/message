/**
 * @file base.cpp
 * @brief Base header file, include all necessary headers and define some common
 * functions.
 *
 * @author salvor
 * @version 0.1
 * @date 2023-02-05
 *
 * Copyright (c) 2023 Salvor
 */

#include "base.h"
#include <fmt/core.h>

void fail(beast::error_code ec, char const *what) {
    fmt::print(stderr, "Error: {} - {}\n", what, ec.message());
}

