#include "session.h"
#include "state.h"
#include <memory>
#include <utility>

Session::Session(tcp::socket &&socket, std::shared_ptr<State> state)
    : ws_(std::move(socket)), state_(std::move(state)),
      buffer_(std::make_shared<beast::flat_buffer>()) {}

Session::~Session() {
    state_->leave(this);
}

void Session::run() {
    asio::dispatch(
        ws_.get_executor(),
        beast::bind_front_handler(&Session::on_run, shared_from_this()));
}

void Session::on_run() {
    // Set suggested timeout settings for the websocket
    ws_.set_option(
        websocket::stream_base::timeout::suggested(beast::role_type::server));

    // Set a decorator to change the Server of the handshake
    ws_.set_option(
        websocket::stream_base::decorator([](websocket::response_type &res) {
            res.set(http::field::server,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                        " message-server-async");
        }));

    // Accept the websocket handshake
    ws_.async_accept(
        beast::bind_front_handler(&Session::on_accept, shared_from_this()));
}

void Session::on_accept(beast::error_code ec) {
    if (ec)
        return fail(ec, "accept");

    state_->join(this);

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
    if (ec == websocket::error::closed)
        return;

    if (ec)
        fail(ec, "read");

    fmt::print(stderr, "read: {}\n", beast::buffers_to_string(buffer_->data()));

    state_->send_to_all(beast::buffers_to_string(buffer_->data()));
    buffer_->consume(buffer_->size());
    do_read();
}

void Session::send(std::string msg) {
    /*if (buf == buffer_) {
        return;
    }*/

    // Post our work to the strand, this ensures
    asio::post(
        ws_.get_executor(),
        beast::bind_front_handler(&Session::on_send, shared_from_this(), msg));
}

void Session::on_send(const std::string &msg) {
    queue_.push(msg);

    // Are we already writing?
    if (queue_.size() > 1) {
        return;
    }

    fmt::print(stderr, "write: {}\n", queue_.front());

    ws_.async_write(
        asio::buffer(queue_.front()),
        beast::bind_front_handler(&Session::on_write, shared_from_this()));
}

void Session::on_write(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec)
        return fail(ec, "write");

    queue_.pop();

    if (!queue_.empty()) {
        ws_.async_write(
            asio::buffer(queue_.front()),
            beast::bind_front_handler(&Session::on_write, shared_from_this()));
    }
}