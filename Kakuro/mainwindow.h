#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QMenu>
#include <memory>
#include "kkrboard.h"
#include "problemdata.h"

namespace pd = problemdata;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    /*
     * Core widges
    */
    // play buttons
    QPushButton *m_pButtonPlay;
    QPushButton *m_pButtonUndo;
    QPushButton *m_pButtonCheck;

    // main board
    KkrBoard * m_pKkrBoard;

    // time indicator
    QProgressBar *m_pPBarTime;
    QLabel       *m_pLabelTime;

    // Menus
    QMenu *m_pMenuView;

    /*
     * dimensions
    */
    static const int BUTTON_WIDTH = 100;
    static const int LABEL_TIME_WIDTH = 50;
    static const int BOARD_WIDTH = 400;
    static const int BOARD_HEIGHT = 300;
    static const int MAIN_WIDTH = 640;
    static const int MAIN_HEIGHT = 480;

    /*
     * initialization
    */
    void makeCoreWidgets();
    void setupCentralPane();
    void setupMainMenu();
    void setupDocks();

private slots:
    void open();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void newData(std::shared_ptr<pd::ProblemData> pData);
};

#endif // MAINWINDOW_H
