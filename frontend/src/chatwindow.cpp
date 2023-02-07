/**
 * @file chatwindow.cpp
 * @brief ChatWindow class implementation
 *
 * @author salvor
 * @date 2023-02-07
 * @version 0.1
 *
 * Copyright (c) 2023 Salvor
 */

#include "chatwindow.h"
#include "chatclient.h"
#include "ui_chatwindow.h"

#include <QApplication>
#include <QHostAddress>
#include <QInputDialog>
#include <QListView>
#include <QMessageBox>
#include <QPalette>
#include <QScrollBar>
#include <QStandardItemModel>
#include <QVariant>
#include <QtCore/qstring.h>
#include <QtGui/qcolor.h>
#include <QtNetwork/qhostaddress.h>
#include <QtWidgets/qscrollbar.h>

ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent), ui_(new Ui::ChatWindow),
      chat_client_(new ChatClient(this)),
      chat_model_(new QStandardItemModel(this)) {
    ui_->setupUi(this);

    ui_->messageEdit->setEnabled(false);
    ui_->sendButton->setEnabled(false);

    chat_model_->insertColumn(0);
    chat_model_->setHeaderData(0, Qt::Horizontal, "Chat");
    ui_->chatView->setModel(chat_model_);

    connect(ui_->sendButton, &QPushButton::clicked, this,
            &ChatWindow::sendMessage);
    connect(ui_->connectButton, &QPushButton::clicked, this,
            &ChatWindow::attemptConnection);
    connect(ui_->messageEdit, &QLineEdit::returnPressed, this,
            &ChatWindow::sendMessage);
    connect(chat_client_, &ChatClient::connected, this, &ChatWindow::connected);
    connect(chat_client_, &ChatClient::disconnected, this,
            &ChatWindow::disconnected);
    connect(chat_client_, &ChatClient::error, this, &ChatWindow::error);
    connect(chat_client_, &ChatClient::loggedIn, this, &ChatWindow::loggedIn);
    connect(chat_client_, &ChatClient::loginError, this,
            &ChatWindow::loginError);
    connect(chat_client_, &ChatClient::userJoined, this,
            &ChatWindow::userJoined);
    connect(chat_client_, &ChatClient::userLeft, this, &ChatWindow::userLeft);
    connect(chat_client_, &ChatClient::messageReceived, this,
            &ChatWindow::messageReceived);
    connect(chat_client_, &ChatClient::userListReceived, this,
            &ChatWindow::userListReceived);
}

ChatWindow::~ChatWindow() {
    delete ui_;
}

void ChatWindow::attemptConnection() {
    const QString host_address =
        QInputDialog::getText(this, tr("Choose Server"), tr("Server Address"),
                              QLineEdit::Normal, QStringLiteral("127.0.0.1"));
    if (host_address.isEmpty()) {
        return;
    }
    ui_->connectButton->setEnabled(false);
    chat_client_->connectToServer(QHostAddress(host_address), 10005);
}

void ChatWindow::connected() {
    const QString user_name =
        QInputDialog::getText(this, tr("Choose Username"), tr("Username"),
                              QLineEdit::Normal, QStringLiteral("test"));
    if (user_name.isEmpty()) {
        ui_->connectButton->setEnabled(true);
        return chat_client_->disconnectFromHost();
    }

    user_name_ = user_name;
    attemptLogin();
}

void ChatWindow::attemptLogin() {
    chat_client_->login(user_name_);
}

void ChatWindow::loggedIn() {
    ui_->connectButton->setEnabled(true);
    ui_->messageEdit->setEnabled(true);
    ui_->sendButton->setEnabled(true);
    ui_->messageEdit->setFocus();

    last_user_.clear();
}

void ChatWindow::loginError(const QString &reason) {
    QMessageBox::warning(this, "Login Error", reason);
    attemptLogin();
}

void ChatWindow::sendMessage() {
    if (ui_->messageEdit->text().isEmpty()) {
        return;
    }

    chat_client_->sendMessage(ui_->messageEdit->text(), user_name_);
    ui_->messageEdit->clear();
}

void ChatWindow::disconnected() {
    ui_->messageEdit->setEnabled(false);
    ui_->sendButton->setEnabled(false);
}

void ChatWindow::error(QAbstractSocket::SocketError socket_error) {
    ui_->connectButton->setEnabled(true);
    ui_->sendButton->setEnabled(false);
    ui_->messageEdit->setEnabled(false);
    ui_->chatView->setEnabled(false);
    last_user_.clear();

    switch (socket_error) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::critical(this, tr("Host Not Found"),
                              tr("The host was not found. Please check the "
                                 "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::critical(this, tr("Connection Refused"),
                              tr("The connection was refused by the peer. "
                                 "Make sure the chat server is running, "
                                 "and check that the host name and port "
                                 "settings are correct."));
        break;
    case QAbstractSocket::SocketTimeoutError:
        QMessageBox::critical(this, tr("Connection Timeout"),
                              tr("The connection timed out. Please check "
                                 "the host name and port settings."));
        break;
    case QAbstractSocket::NetworkError:
        QMessageBox::critical(this, tr("Network Error"),
                              tr("An error occurred while attempting to "
                                 "connect to the server. Please check the "
                                 "host name and port settings."));
        break;
    case QAbstractSocket::AddressInUseError:
        QMessageBox::critical(this, tr("Address In Use"),
                              tr("The address is already in use. Please "
                                 "make sure that no other clients are "
                                 "using the same address and port."));
        break;
    case QAbstractSocket::SocketAccessError:
        QMessageBox::critical(this, tr("Socket Access Error"),
                              tr("The socket operation failed because the "
                                 "application lacked the required "
                                 "privileges."));
        break;
    case QAbstractSocket::DatagramTooLargeError:
        QMessageBox::critical(this, tr("Datagram Too Large"),
                              tr("The datagram was larger than the "
                                 "operating system's limit. Please contact "
                                 "your system administrator."));
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        QMessageBox::critical(this, tr("Address Not Available"),
                              tr("The address specified to "
                                 "QUdpSocket::bind() does not belong to "
                                 "the host."));
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        QMessageBox::critical(this, tr("Unsupported Socket Operation"),
                              tr("The requested socket operation is not "
                                 "supported by the local operating system "
                                 "(e.g., lack of IPv6 support)."));
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        QMessageBox::critical(this, tr("Proxy Authentication Required"),
                              tr("The socket is using a proxy, and the "
                                 "proxy requires authentication."));
        break;
    case QAbstractSocket::SslHandshakeFailedError:
        QMessageBox::critical(this, tr("SSL Handshake Failed"),
                              tr("The SSL/TLS handshake failed, so the "
                                 "connection was closed (only used in "
                                 "QSslSocket)"));
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        QMessageBox::critical(this, tr("Unfinished Socket Operation"),
                              tr("Used by QAbstractSocketEngine only, "
                                 "The last operation attempted has not "
                                 "finished yet (still in progress in the "
                                 "background)."));
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        QMessageBox::critical(this, tr("Proxy Connection Refused"),
                              tr("Could not contact the proxy server "
                                 "because the connection to that server "
                                 "was denied"));
        break;
    case QAbstractSocket::ProxyConnectionClosedError:
        QMessageBox::critical(this, tr("Proxy Connection Closed"),
                              tr("The connection to the proxy server "
                                 "was closed unexpectedly (before the "
                                 "connection to the final peer was "
                                 "established)"));
        break;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        QMessageBox::critical(this, tr("Proxy Connection Timeout"),
                              tr("The connection to the proxy server "
                                 "timed out or the proxy server stopped "
                                 "responding in the authentication phase."));
        break;
    case QAbstractSocket::ProxyNotFoundError:
        QMessageBox::critical(this, tr("Proxy Not Found"),
                              tr("The proxy address set with "
                                 "setProxy() (or the application proxy) "
                                 "was not found."));
        break;
    case QAbstractSocket::ProxyProtocolError:
        QMessageBox::critical(this, tr("Proxy Protocol Error"),
                              tr("The connection negotiation with the "
                                 "proxy server failed, because the "
                                 "response from the proxy server could "
                                 "not be understood."));
        break;
    case QAbstractSocket::OperationError:
        QMessageBox::critical(this, tr("Operation Error"),
                              tr("An operation was attempted while the "
                                 "socket was in a state that did not "
                                 "permit it."));
        break;
    case QAbstractSocket::SslInternalError:
        QMessageBox::critical(this, tr("SSL Internal Error"),
                              tr("The SSL library being used reported an "
                                 "internal error. This is probably the "
                                 "result of a bad installation or "
                                 "misconfiguration of the library."));
        break;
    case QAbstractSocket::SslInvalidUserDataError:
        QMessageBox::critical(this, tr("SSL Invalid User Data"),
                              tr("Invalid data (certificate, key, cypher, "
                                 "etc.) was provided and its use resulted "
                                 "in an error in the SSL library."));
        break;
    case QAbstractSocket::TemporaryError:
        QMessageBox::critical(this, tr("Temporary Error"),
                              tr("A temporary error occurred (e.g., "
                                 "operation would block and socket is "
                                 "non-blocking)."));
        break;
    case QAbstractSocket::UnknownSocketError:
        QMessageBox::critical(this, tr("Unknown Socket Error"),
                              tr("An unidentified error occurred."));
        break;
    default:
        QMessageBox::critical(this, tr("Unknown Error"),
                              tr("An unidentified error occurred."));
        break;
    }
}

void ChatWindow::messageReceived(const QString &sender,
                                 const QString &message) {
    int new_row = chat_model_->rowCount();

    if (sender == user_name_) {
        chat_model_->insertRow(new_row);
        chat_model_->setData(chat_model_->index(new_row, 0), message);
        chat_model_->setData(chat_model_->index(new_row, 0), Qt::AlignRight,
                             Qt::TextAlignmentRole);
        chat_model_->setData(chat_model_->index(new_row, 0), QColor(53, 57, 69),
                             Qt::ForegroundRole);
        ui_->chatView->scrollToBottom();
        return;
    }

    if (sender != last_user_) {
        last_user_ = sender;

        QFont bold_font;
        bold_font.setBold(true);
        chat_model_->insertRows(new_row, 2);
        chat_model_->setData(chat_model_->index(new_row, 0), sender + ":");
        chat_model_->setData(chat_model_->index(new_row, 0), bold_font,
                             Qt::FontRole);
        chat_model_->setData(chat_model_->index(new_row, 0), Qt::AlignLeft,
                             Qt::TextAlignmentRole);
        chat_model_->setData(chat_model_->index(new_row, 0), QColor(94, 90, 91),
                             Qt::ForegroundRole);
        new_row++;
    } else {
        chat_model_->insertRow(new_row);
    }

    chat_model_->setData(chat_model_->index(new_row, 0), message);
    chat_model_->setData(chat_model_->index(new_row, 0), Qt::AlignLeft,
                         Qt::TextAlignmentRole);
    chat_model_->setData(chat_model_->index(new_row, 0), QColor(53, 57, 69),
                         Qt::ForegroundRole);
    ui_->chatView->scrollToBottom();
}

void ChatWindow::userJoined(const QString &user) {
    int new_row = chat_model_->rowCount();
    chat_model_->insertRow(new_row);
    chat_model_->setData(chat_model_->index(new_row, 0), user + " joined");
    chat_model_->setData(chat_model_->index(new_row, 0), Qt::AlignCenter,
                         Qt::TextAlignmentRole);
    chat_model_->setData(chat_model_->index(new_row, 0), QColor(53, 57, 69),
                         Qt::ForegroundRole);
    ui_->chatView->scrollToBottom();
}

void ChatWindow::userLeft(const QString &user) {
    int new_row = chat_model_->rowCount();
    chat_model_->insertRow(new_row);
    chat_model_->setData(chat_model_->index(new_row, 0), user + " left");
    chat_model_->setData(chat_model_->index(new_row, 0), Qt::AlignCenter,
                         Qt::TextAlignmentRole);
    chat_model_->setData(chat_model_->index(new_row, 0), QColor(53, 57, 69),
                         Qt::ForegroundRole);
    ui_->chatView->scrollToBottom();
}

void ChatWindow::userListReceived(const QStringList &user_list) {
    QString user_list_string = "Online users: ";
    for (const QString &user : user_list) {
        user_list_string += user + ", ";
    }

    int new_row = chat_model_->rowCount();
    chat_model_->insertRow(new_row);
    chat_model_->setData(chat_model_->index(new_row, 0), user_list_string);
    chat_model_->setData(chat_model_->index(new_row, 0), Qt::AlignCenter,
                         Qt::TextAlignmentRole);
    chat_model_->setData(chat_model_->index(new_row, 0), QColor(53, 57, 69),
                         Qt::ForegroundRole);
    ui_->chatView->scrollToBottom();
}