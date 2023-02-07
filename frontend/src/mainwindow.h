/**
 * @file mainwindow.h
 * @brief Main window of the application
 * @details Main window of the application, create a QWebEngineView object, and
 * load a web page.
 * @see QWebEngineView
 *
 * @author salvor
 * @version 0.1
 * @date 2023-02-05
 *
 * Copyright (c) 2023 Salvor
 */

#pragma once

#include <QMainWindow>

/**
 * @brief Main window of the application
 * @details Main window of the application, create a QWebEngineView object, and
 * load a web page. The main window has a menu bar, and a QWebEngineView object.
 * @see QWebEngineView
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    /**
     * @brief Constructor
     * @details Constructor, create a QWebEngineView object, and load a web
     * page.
     *
     * @param parent The parent widget.
     */
    explicit MainWindow(QWidget *parent = nullptr);
    /**
     * @brief Destructor
     * @details Destructor, delete the QWebEngineView object.
     */
    

  private slots:
    /**
     * @brief Create a new window
     * @details Create a new window, and load a web page. It can create multiple
     * sessions to connect to the server and chat.
     */
    void new_window();
    /**
     * @brief Show the about dialog
     * @details Show the about dialog.
     */
    void about();

  private:
    /**
     * @brief Create actions
     * @details Create actions, and add them to the menu bar.
     */
    void create_actions();
};