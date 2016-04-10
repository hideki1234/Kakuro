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
    m_pButtonPlay = new QPushButton{tr("Play")};
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
    connect(this, &MainWindow::newData, m_pKkrBoard, &KkrBoard::updateData);
    connect(this, &MainWindow::newData, &m_ps, &playstatus::PlayStatus::updateData);

    // button signals
    connect(m_pButtonPlay, &QPushButton::clicked, &m_ps, &ps::PlayStatus::playPressed);

    // play status change
    connect(&m_ps, &playstatus::PlayStatus::statusChanged, m_pKkrBoard, &KkrBoard::updateStatus);
    connect(&m_ps, &playstatus::PlayStatus::statusChanged, this, &MainWindow::updateStatus);

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

    emit newData(pData);
}

void MainWindow::updateStatus(playstatus::Status newStatus)
{
    static const QString sPlay{tr("Play")};
    static const QString sPause{tr("Pause")};
    static const QString sResume{tr("Resume")};

    switch(newStatus) {
    case ps::Status::NODATA:
        m_pButtonPlay->setText(sPlay);
        m_pButtonPlay->setEnabled(false);
        m_pButtonCheck->setEnabled(false);
        m_secTimer.stop();
        setTimeIndicator(true);
        break;
    case ps::Status::READY:
        m_pButtonPlay->setText(sPlay);
        m_pButtonPlay->setEnabled(true);
        m_pButtonCheck->setEnabled(false);
        m_secTimer.stop();
        setTimeIndicator(true);
        break;
    case ps::Status::INPLAY:
        m_pButtonPlay->setText(sPause);
        m_pButtonCheck->setEnabled(true);
        setTimeIndicator();
        m_secTimer.start(TIMER_INTERVAL);
        break;
    case ps::Status::PAUSED:
        m_pButtonPlay->setText(sResume);
        m_pButtonCheck->setEnabled(false);
        m_secTimer.stop();
        break;
    case ps::Status::DONE:
        m_pButtonPlay->setText(sPlay);
        m_pButtonPlay->setEnabled(false);
        m_pButtonCheck->setEnabled(false);
        m_secTimer.stop();
        setTimeIndicator();
        break;
    }
}

void MainWindow::timeout()
{
    setTimeIndicator();
}
