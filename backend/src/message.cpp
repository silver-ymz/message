/**
 * @file message.cpp
 * @brief Message class implementation
 *
 * @author salvor
 * @version 0.1
 * @date 2023-02-07
 *
 * Copyright (c) 2023 Salvor
 */

#include "message.h"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>

Message::Message(const std::string &message) {
    document_.Parse(message.c_str());
}

std::string Message::stringify() const {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document_.Accept(writer);
    return buffer.GetString();
}

bool Message::is_login_info() const {
    return document_["type"] == "login";
}

bool Message::is_chat_message() const {
    return document_["type"] == "message";
}

std::string Message::get_username() const {
    return document_["username"].GetString();
}

std::string Message::get_message() const {
    return document_["text"].GetString();
}