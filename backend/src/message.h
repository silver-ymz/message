/**
 * @file message.h
 * @brief Message class. Parse and stringify message.
 *
 * @author salvor
 * @version 0.1
 * @date 2023-02-07
 *
 * Copyright (c) 2023 Salvor
 */

#pragma once

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>

/**
 * @brief Message class. Parse and stringify message.
 * @details Message class. Parse and stringify message. Message is a JSON
 * string. Message class use rapidjson to parse and stringify message.
 */
class Message {
  public:
    /**
     * @brief Construct a new Message object
     *
     * @param message Message string, in JSON format.
     */
    explicit Message(const std::string &message);

    /**
     * @brief Stringify message.
     *
     * @return std::string Stringified message.
     */
    [[nodiscard]] std::string stringify() const;

    /**
     * @brief Check if message is login info.
     *
     * @return true Message is login info.
     * @return false Message is not login info.
     */
    [[nodiscard]] bool is_login_info() const;
    /**
     * @brief Check if message is chat message.
     *
     * @return true Message is chat message.
     * @return false Message is not chat message.
     */
    [[nodiscard]] bool is_chat_message() const;

    /**
     * @brief Get username from login info.
     *
     * @return std::string Username.
     */
    [[nodiscard]] std::string get_username() const;
    /**
     * @brief Get message from chat message.
     *
     * @return std::string Message.
     */
    [[nodiscard]] std::string get_message() const;

  private:
    /**
     * @brief Rapidjson document.
     */
    rapidjson::Document document_;
};