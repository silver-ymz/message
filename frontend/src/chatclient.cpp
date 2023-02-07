/**
 * @file chatclient.cpp
 * @brief ChatClient class implementation
 *
 * @author salvor
 * @date 2023-02-07
 * @version 0.1
 *
 * Copyright (c) 2023 Salvor
 */

#include "chatclient.h"

#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QMessageBox>

ChatClient::ChatClient(QObject *parent)
    : QObject(parent), client_socket_(new QWebSocket), logged_in_(false) {
    connect(client_socket_, &QWebSocket::textMessageReceived, this,
            &ChatClient::onReadyRead);
    connect(client_socket_, &QWebSocket::connected, this,
            &ChatClient::connected);
    connect(client_socket_, &QWebSocket::disconnected, this,
            &ChatClient::disconnected);
    connect(client_socket_,
            QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            this, &ChatClient::error);
}

void ChatClient::connectToServer(const QHostAddress &address, quint16 port) {
    client_socket_->open(
        QUrl("ws://" + address.toString() + ":" + QString::number(port)));
}

void ChatClient::login(const QString &user_name) {
    QJsonObject login;
    login["type"] = "login";
    login["username"] = user_name;
    QJsonDocument doc(login);
    client_socket_->sendTextMessage(doc.toJson());
}

void ChatClient::sendMessage(const QString &text, const QString &user_name_) {
    QJsonObject message;
    message["type"] = "message";
    message["sender"] = user_name_;
    message["text"] = text;
    QJsonDocument doc(message);
    client_socket_->sendTextMessage(doc.toJson());
}

void ChatClient::disconnectFromHost() {
    client_socket_->close();
}

void ChatClient::onReadyRead(const QString &message) {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
        QMessageBox::warning(nullptr, "Error", error.errorString());
        return;
    }
    jsonReceived(doc.object());
}

void ChatClient::jsonReceived(const QJsonObject &doc) {
    if (doc["type"] == "login") {
        if (doc["success"].toBool()) {
            logged_in_ = true;
            emit loggedIn();
        } else {
            emit loginError(doc["reason"].toString());
        }
    } else if (doc["type"] == "message") {
        emit messageReceived(doc["sender"].toString(), doc["text"].toString());
    } else if (doc["type"] == "user_joined") {
        emit userJoined(doc["username"].toString());
    } else if (doc["type"] == "user_left") {
        emit userLeft(doc["username"].toString());
    }

    if (doc["type"] == "user_list") {
        QJsonArray users = doc["users"].toArray();
        QStringList user_list;
        for (const auto user : users) {
            user_list.append(user.toString());
        }
        emit userListReceived(user_list);
    }
}