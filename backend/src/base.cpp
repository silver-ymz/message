#include "base.h"
#include <fmt/core.h>

void fail(beast::error_code ec, char const *what) {
    fmt::print(stderr, "{}: {}\n", what, ec.message());
}