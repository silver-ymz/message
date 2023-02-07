/**
 * @file chatclient.h
 * @brief ChatClient class declaration
 *
 * @author salvor
 * @date 2023-02-07
 * @version 0.1
 *
 * Copyright (c) 2023 Salvor
 */

#pragma once

#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QMessageBox>
#include <QObject>
#include <QWebSocket>

/**
 * @brief The ChatClient class
 * @details This class is responsible for connecting to the server and
 * sending/receiving messages. It also emits signals when certain events
 * happen. The signals are connected to slots in the ChatWindow class.
 */
class ChatClient : public QObject {
    Q_OBJECT

  public:
    /**
     * @brief ChatClient constructor
     * @details Creates a new QWebSocket and connects the signals to slots.
     * @param parent
     */
    explicit ChatClient(QObject *parent = nullptr);

  signals:
    /**
     * @brief Signals emitted when the client connects to the server
     * @details These signals are connected to slots in the ChatWindow class.
     * They are used to update the GUI.
     */
    void connected();
    /**
     * @brief Signals emitted when the client logs in
     * @details These signals are connected to slots in the ChatWindow class.
     * They are used to update the GUI.
     */
    void loggedIn();
    /**
     * @brief Signals emitted when the client fails to log in
     * @details These signals are connected to slots in the ChatWindow class.
     * It is used to display an error message. The reason for the error is
     * passed as a parameter.
     *
     * @param reason The reason for the error
     */
    void loginError(const QString &reason);
    /**
     * @brief Signals emitted when the client disconnects from the server
     * @details These signals are connected to slots in the ChatWindow class.
     * They are used to update the GUI.
     */
    void disconnected();
    /**
     * @brief Signals emitted when the client receives a message
     * @details These signals are connected to slots in the ChatWindow class.
     * They are used to update the GUI.
     *
     * @param sender The sender of the message
     * @param text The text of the message
     */
    void messageReceived(const QString &sender, const QString &text);
    /**
     * @brief Signals emitted when the client receives an error
     * @details These signals are connected to slots in the ChatWindow class.
     * It is used to display an error message. The reason for the error is
     * passed as a parameter.
     *
     * @param socket_error The reason for the error
     */
    void error(QAbstractSocket::SocketError socket_error);
    /**
     * @brief Signals emitted when the client receives a message about
     * a user joining
     * @details These signals are connected to slots in the ChatWindow class.
     * It will print a message in the chat window. The username of the user
     * who joined is passed as a parameter.
     *
     * @param username The username of the user who joined
     */
    void userJoined(const QString &username);
    /**
     * @brief Signals emitted when the client receives a message about
     * a user leaving
     * @details These signals are connected to slots in the ChatWindow class.
     * It will print a message in the chat window. The username of the user
     * who left is passed as a parameter.
     *
     * @param username The username of the user who left
     */
    void userLeft(const QString &username);
    /**
     * @brief Signals emitted when the client receives a list of users
     * @details These signals are connected to slots in the ChatWindow class.
     * They are used to update the GUI.
     *
     * @param user_list The list of users
     */
    void userListReceived(const QStringList &user_list);

  public slots:
    /**
     * @brief Connects to the server
     * @details This function connects to the server. The address and port
     * are passed as parameters.
     *
     * @param address The address of the server
     * @param port The port of the server
     */
    void connectToServer(const QHostAddress &address, quint16 port);
    /**
     * @brief Logs in to the server
     * @details This function logs in to the server. The username is passed
     * as a parameter.
     *
     * @param user_name The username
     */
    void login(const QString &user_name);
    /**
     * @brief Sends a message to the server
     * @details This function sends a message to the server. The message and
     * the username are passed as parameters.
     *
     * @param text The message
     * @param user_name_ The username
     */
    void sendMessage(const QString &text, const QString &user_name_);
    /**
     * @brief Disconnects from the server
     * @details This function disconnects from the server.
     */
    void disconnectFromHost();

  private slots:
    /**
     * @brief Called when the client receives a message
     * @details This function is called when the client receives a message.
     * It parses the message and emits the appropriate signals.
     *
     * @param message The message received
     */
    void onReadyRead(const QString &message);

  private:
    /**
     * @brief A pointer to the QWebSocket object
     * @details This pointer is used to connect to the server. It is
     * initialized in the constructor.
     */
    QWebSocket *client_socket_;
    /**
     * @brief A boolean indicating whether the client is logged in
     * @details This boolean is used to check whether the client is logged in.
     * It is initialized to false in the constructor.
     */
    bool logged_in_;

    /**
     * @brief Parses a JSON message received from the server
     * @details This function parses a JSON message received from the server. It
     * emits the appropriate signals.
     *
     * @param doc The JSON document received
     */
    void jsonReceived(const QJsonObject &doc);
};