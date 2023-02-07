/**
 * @file mainwindow.cpp
 * @brief MainWindow class implementation
 * @details MainWindow class implementation, create a QWebEngineView object, and
 * load a web page.
 * @see QWebEngineView
 *
 * @author salvor
 * @version 0.1
 * @date 2023-02-05
 *
 * Copyright (c) 2023 Salvor
 */

#include "mainwindow.h"
#include "chatwindow.h"

#include <QApplication>
#include <QMenuBar>
#include <QMessageBox>
#include <QtWidgets/qaction.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    create_actions();

    setWindowTitle(tr("Chat Client"));

    auto *chat_window = new ChatWindow(this);
    setCentralWidget(chat_window);
}

void MainWindow::create_actions() {
    QMenu *file_menu = menuBar()->addMenu(tr("&Window"));
    auto *new_act = new QAction(tr("&New"), this);
    new_act->setShortcuts(QKeySequence::New);
    new_act->setStatusTip(tr("Create a new window"));
    connect(new_act, &QAction::triggered, this, &MainWindow::new_window);
    file_menu->addAction(new_act);

    QAction *close_act =
        file_menu->addAction(tr("&Close"), this, &QWidget::close);
    close_act->setShortcuts(QKeySequence::Close);
    close_act->setStatusTip(tr("Close the active window"));

    file_menu->addSeparator();

    const QIcon exit_icon = QIcon::fromTheme("application-exit");
    QAction *exit_act =
        file_menu->addAction(exit_icon, tr("&Exit"), this, &QWidget::close);
    exit_act->setShortcuts(QKeySequence::Quit);
    exit_act->setStatusTip(tr("Exit the application"));

    QMenu *help_menu = menuBar()->addMenu(tr("&Help"));
    QAction *about_act =
        help_menu->addAction(tr("&About"), this, &MainWindow::about);
    about_act->setStatusTip(tr("Show the application's About box"));

    QAction *about_qt_act =
        help_menu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    about_qt_act->setStatusTip(tr("Show the Qt library's About box"));
}

void MainWindow::new_window() {
    auto *window = new MainWindow(this);
    window->show();
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About Application"),
                       tr("This is a simple chat application."));
}