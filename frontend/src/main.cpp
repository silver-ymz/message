/**
 * @file main.cpp
 * @brief Main function
 * @details Main function, create a QApplication object, and create a MainWindow
 * object.
 * @see MainWindow
 *
 * @author salvor
 * @version 0.1
 * @date 2023-02-05
 *
 * Copyright (c) 2023 Salvor
 */

#include "mainwindow.h"
#include <QApplication>

/**
 * @brief Main function.
 * @details Main function, create a QApplication object, and create a MainWindow
 * object. Then call QApplication::exec() to start the event loop.
 *
 * @param argc The number of arguments.
 * @param argv The arguments.
 * @return int The return code.
 */
int main(int argc, char **argv) {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return QApplication::exec();
}