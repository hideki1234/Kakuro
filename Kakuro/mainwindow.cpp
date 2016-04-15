#include "mainwindow.h"
#include <QFont>
#include <QWidget>
#include <QBoxLayout>
#include <QScrollArea>
#include <QMenuBar>
#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>
#include "problemdata.h"
#include <QDebug>

/*
 * MainWindow implementation
 */
// initializations
void MainWindow::makeCoreWidgets()
{
    m_pButtonPlay = new QPushButton{tr("Start")};
    m_pButtonPlay->setMinimumWidth(BUTTON_WIDTH);
    m_pButtonPlay->setEnabled(false);
    m_pButtonUndo = new QPushButton{tr("Undo")};
    m_pButtonUndo->setMinimumWidth(BUTTON_WIDTH);
    m_pButtonUndo->setEnabled(false);
    m_pButtonCheck = new QPushButton{tr("Check")};
    m_pButtonCheck->setMinimumWidth(BUTTON_WIDTH);
    m_pButtonCheck->setEnabled(false);

    m_pKkrBoard = new KkrBoard;

    m_pPBarTime = new QProgressBar;
    m_pPBarTime->setAlignment(Qt::AlignHCenter);
    m_pPBarTime->reset();
    m_pLabelTime = new QLabel{tr("----:--")};
    m_pLabelTime->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QFont fontFixed(tr("Courier"));
    m_pLabelTime->setFont(fontFixed);
    m_pLabelTime->setMinimumWidth(LABEL_TIME_WIDTH);
}

void MainWindow::setupCentralPane()
{
    QWidget *pWCenter = new QWidget;
    QBoxLayout *pBLCenter = new QBoxLayout{QBoxLayout::TopToBottom};
    pWCenter->setLayout(pBLCenter);

    // buttons
    QBoxLayout *pBLButtons = new QBoxLayout{QBoxLayout::LeftToRight};
    pBLButtons->addWidget(m_pButtonPlay);
    pBLButtons->addWidget(m_pButtonUndo);
    pBLButtons->addWidget(m_pButtonCheck);
    pBLButtons->addStretch();
    pBLCenter->addLayout(pBLButtons);

    // main board
    QScrollArea *pScrollBoard = new QScrollArea;
    QPalette palBack(palette());
    palBack.setColor(QPalette::Background, Qt::white);
    pScrollBoard->setPalette(palBack);
    pScrollBoard->setAutoFillBackground(true);
    pScrollBoard->setMinimumWidth(BOARD_WIDTH);
    pScrollBoard->setMinimumHeight(BOARD_HEIGHT);
    pScrollBoard->setWidget(m_pKkrBoard);
    pBLCenter->addWidget(pScrollBoard);

    // time indicator
    QBoxLayout *pBLTime = new QBoxLayout{QBoxLayout::LeftToRight};
    pBLTime->addWidget(m_pPBarTime);
    pBLTime->addWidget(m_pLabelTime);
    pBLCenter->addLayout(pBLTime);

    setCentralWidget(pWCenter);
}

void MainWindow::setupMainMenu()
{
    QMenuBar *pMainMenu = new QMenuBar;

    // File menu
    QMenu * pMenuFile = new QMenu{tr("&File")};
    pMenuFile->addAction(tr("&Open"), this, &MainWindow::open);
    pMenuFile->addSeparator();
    pMenuFile->addAction(tr("E&xit"), this, &QWidget::close);
    pMainMenu->addMenu(pMenuFile);

    // Play
    QMenu * pMenuPlay = new QMenu{tr("&Play")};
    m_pActionPlay = pMenuPlay->addAction(tr("&Start"), &m_ps, &ps::PlayStatus::playPressed);
    m_pActionUndo = pMenuPlay->addAction(tr("&Undo"));
    m_pActionCheck = pMenuPlay->addAction(tr("Chec&k"));
    pMenuPlay->addSeparator();
    m_pActionGiveup = pMenuPlay->addAction(tr("Give &up"), this, &MainWindow::makeSureGiveup);
    m_pActionPlay->setEnabled(false);
    m_pActionUndo->setEnabled(false);
    m_pActionCheck->setEnabled(false);
    m_pActionGiveup->setEnabled(false);
    pMainMenu->addMenu(pMenuPlay);

    // View
    m_pMenuView = new QMenu{tr("&View")};
    pMainMenu->addMenu(m_pMenuView);

    setMenuBar(pMainMenu);
}

void MainWindow::setupDocks()
{
    QDockWidget *pDockBirdsEye = new QDockWidget{tr("Bird's Eye"), this};
    pDockBirdsEye->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_pMenuView->addAction(pDockBirdsEye->toggleViewAction());
    addDockWidget(Qt::RightDockWidgetArea, pDockBirdsEye);

    QDockWidget *pDockBookmarks = new QDockWidget{tr("Bookmarks"), this};
    pDockBookmarks->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_pMenuView->addAction(pDockBookmarks->toggleViewAction());
    addDockWidget(Qt::RightDockWidgetArea, pDockBookmarks);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    makeCoreWidgets();
    setupCentralPane();
    setupMainMenu();
    setupDocks();

    /*
     * connections
     */
    // signal in loading data
    connect(this, &MainWindow::newProblem, m_pKkrBoard, &KkrBoard::updateProblem);
    connect(this, &MainWindow::newProblem, &m_ps, &playstatus::PlayStatus::updateProblem);
    connect(this, &MainWindow::newProblem, &m_uam, &ua::UserAnswerManager::updateProblem);

    // button signals
    connect(m_pButtonPlay, &QPushButton::clicked, &m_ps, &ps::PlayStatus::playPressed);

    // play status change
    connect(&m_ps, &playstatus::PlayStatus::statusChanged, m_pKkrBoard, &KkrBoard::updateStatus);
    connect(&m_ps, &playstatus::PlayStatus::statusChanged, this, &MainWindow::updateStatus);
    connect(this, &MainWindow::giveup, &m_ps, &playstatus::PlayStatus::giveup);

    // user answer signals
    connect(&m_uam, &ua::UserAnswerManager::newUserAnswer, m_pKkrBoard, &KkrBoard::updateUserAnswer);
    connect(m_pKkrBoard, &KkrBoard::newAnswerInput, &m_uam, &ua::UserAnswerManager::updateCellAnswer);
    connect(&m_uam, &ua::UserAnswerManager::newCellAnswer, m_pKkrBoard, &KkrBoard::renderAnswer);

    // timer
    connect(&m_secTimer, &QTimer::timeout, this, &MainWindow::timeout);

    // default window size
    setMinimumWidth(MAIN_WIDTH);
    setMinimumHeight(MAIN_HEIGHT);
}

MainWindow::~MainWindow()
{

}

namespace pd = problemdata;
namespace ps = playstatus;

/*
 * plain methods
 */
void MainWindow::setTimeIndicator(bool bNone)
{
    static const QString sNone{tr("----:--")};
    if(bNone)
        m_pLabelTime->setText(sNone);
    else {
        const qint64 totalSec = m_ps.getElapsedTime() / 1000;
        const int min = totalSec / 60;
        const int sec = totalSec % 60;
        const QString sElapsed = QString::asprintf("%4d:%02d", min, sec);
        m_pLabelTime->setText(sElapsed);
    }
}

/*
 * slots
 */
void MainWindow::open()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open Kakuro Data"),
                                                    QString(),
                                                    "Kakuro (*.kkr)",
                                                    nullptr,
                                                    QFileDialog::ReadOnly);
    if(filename == QStringLiteral(""))
        return;

    std::shared_ptr<pd::ProblemData> pData{pd::ProblemData::problemLoader(filename)};
    if(pData == nullptr) {
        QMessageBox::critical(this, tr("Kakuro Player"), tr("Failed to open ") + filename);
        return;
    }

    emit newProblem(pData);
}

void MainWindow::updateStatus(playstatus::Status newStatus)
{
    static const QString sStartB{tr("Start")};
    static const QString sPauseB{tr("Pause")};
    static const QString sResumeB{tr("Resume")};
    static const QString sStartM{tr("&Start")};
    static const QString sPauseM{tr("&Pause")};
    static const QString sResumeM{tr("&Resume")};

    switch(newStatus) {
    case ps::Status::NODATA:
        m_pButtonPlay->setText(sStartB);
        m_pButtonPlay->setEnabled(false);
        m_pButtonCheck->setEnabled(false);
        m_pActionPlay->setText(sStartM);
        m_pActionPlay->setEnabled(false);
        m_pActionCheck->setEnabled(false);
        m_pActionGiveup->setEnabled(false);
        m_secTimer.stop();
        setTimeIndicator(true);
        break;
    case ps::Status::READY:
        m_pButtonPlay->setText(sStartB);
        m_pButtonPlay->setEnabled(true);
        m_pButtonCheck->setEnabled(false);
        m_pActionPlay->setText(sStartM);
        m_pActionPlay->setEnabled(true);
        m_pActionCheck->setEnabled(false);
        m_pActionGiveup->setEnabled(false);
        m_secTimer.stop();
        setTimeIndicator(true);
        break;
    case ps::Status::INPLAY:
        m_pButtonPlay->setText(sPauseB);
        m_pButtonCheck->setEnabled(true);
        m_pActionPlay->setText(sPauseM);
        m_pActionCheck->setEnabled(true);
        m_pActionGiveup->setEnabled(true);
        setTimeIndicator();
        m_secTimer.start(TIMER_INTERVAL);
        break;
    case ps::Status::PAUSED:
        m_pButtonPlay->setText(sResumeB);
        m_pButtonCheck->setEnabled(false);
        m_pActionPlay->setText(sResumeM);
        m_pActionCheck->setEnabled(false);
        m_secTimer.stop();
        break;
    case ps::Status::DONE:
        m_pButtonPlay->setText(sStartB);
        m_pButtonPlay->setEnabled(false);
        m_pButtonCheck->setEnabled(false);
        m_pActionPlay->setEnabled(false);
        m_pActionCheck->setEnabled(false);
        m_pActionGiveup->setEnabled(false);
        m_secTimer.stop();
        setTimeIndicator();
        break;
    }
}

void MainWindow::timeout()
{
    setTimeIndicator();
}

void MainWindow::makeSureGiveup()
{
    const auto ans = QMessageBox::question(this, tr("Kakuro"), tr("Give up? Really!?"));
    if(ans == QMessageBox::Yes)
        emit giveup();
}
