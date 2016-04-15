#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QMenu>
#include <QTimer>
#include <memory>
#include "kkrboard.h"
#include "problemdata.h"
#include "playstatus.h"
#include "useranswermanager.h"

namespace pd = problemdata;
namespace ps = playstatus;
namespace ua = useranswer;

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

    // Actions
    QAction *m_pActionPlay;
    QAction *m_pActionUndo;
    QAction *m_pActionCheck;
    QAction *m_pActionGiveup;

    QTimer m_secTimer;

    /*
     * models
     */
    ps::PlayStatus m_ps;
    ua::UserAnswerManager m_uam;

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

    /*
     * timer
     */
    static const int TIMER_INTERVAL = 500;
    void setTimeIndicator(bool bNone = false);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void open();
    void updateStatus(playstatus::Status newStatus);
    void timeout();
    void makeSureGiveup();

signals:
    void newProblem(std::shared_ptr<pd::ProblemData> pData);
    void giveup();
};

#endif // MAINWINDOW_H
