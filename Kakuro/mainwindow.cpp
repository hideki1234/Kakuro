#include "mainwindow.h"
#include <QFont>
#include <QFontMetrics>
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
    m_pLabelTime = new QLabel{tr("----:--"), this};
    m_pLabelTime->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QFont fontFixed{tr("Courier")};
    m_pLabelTime->setFont(fontFixed);
    m_pLabelTime->setMinimumWidth(LABEL_TIME_WIDTH);

    // floating message
    QFont fontMsg;
    fontMsg.setPointSize(36);
    m_pFloatingMsg = new QLabel{this};
    m_pFloatingMsg->setEnabled(false);
    m_pFloatingMsg->setVisible(false);
    m_pFloatingMsg->setFont(fontMsg);
    m_pFloatingMsg->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QPalette fontPal(palette());
    fontPal.setColor(QPalette::Foreground, Qt::blue);
    m_pFloatingMsg->setPalette(fontPal);
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
    m_pScrollKkr = new QScrollArea;
    QPalette palBack(palette());
    palBack.setColor(QPalette::Background, Qt::white);
    m_pScrollKkr->setPalette(palBack);
    m_pScrollKkr->setAutoFillBackground(true);
    m_pScrollKkr->setMinimumWidth(BOARD_WIDTH);
    m_pScrollKkr->setMinimumHeight(BOARD_HEIGHT);
    m_pScrollKkr->setWidget(m_pKkrBoard);
    m_pScrollKkr->installEventFilter(this);
    pBLCenter->addWidget(m_pScrollKkr);
    m_pKkrBoard->setScrollArea(m_pScrollKkr);

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
    m_pActionPlay->setShortcutContext(Qt::ApplicationShortcut);
    m_pActionUndo = pMenuPlay->addAction(tr("&Undo"));
    m_pActionUndo->setShortcutContext(Qt::ApplicationShortcut);
    m_pActionUndo->setShortcut(Qt::Key_U | Qt::ControlModifier);
    m_pActionCheck = pMenuPlay->addAction(tr("Chec&k"), this, &MainWindow::checkIt);
    m_pActionCheck->setShortcutContext(Qt::ApplicationShortcut);
    m_pActionCheck->setShortcut(Qt::Key_K | Qt::ControlModifier);
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
    connect(m_pButtonCheck, &QPushButton::clicked, this, &MainWindow::checkIt);

    // play status change
    connect(&m_ps, &playstatus::PlayStatus::statusChanged, m_pKkrBoard, &KkrBoard::updateStatus);
    connect(&m_ps, &playstatus::PlayStatus::statusChanged, this, &MainWindow::updateStatus);
    connect(this, &MainWindow::solved, &m_ps, &playstatus::PlayStatus::solved);
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

void MainWindow::showStatusMsg()
{
    setWindowTitle(tr("Kakuro"));
    m_pFloatingMsg->setEnabled(false);
    m_pFloatingMsg->setVisible(false);
}

void MainWindow::showStatusMsg(const QString &statusMsg)
{
    // set the message text
    QString msg{tr("Kakuro - ")};
    msg += statusMsg;
    setWindowTitle(msg);
    m_pFloatingMsg->setText(statusMsg);

    // get the size of the message
    QFontMetrics fm{m_pFloatingMsg->font()};
    //const QRect ms{fm.boundingRect(msg)};
    //m_pMessageLabel->resize(ms.width()/2,ms.height()/2);
    const QSize ms{fm.size(Qt::TextExpandTabs, msg)};
    m_pFloatingMsg->resize(ms);

    // show the message
    m_pFloatingMsg->raise();
    m_pFloatingMsg->setEnabled(true);
    m_pFloatingMsg->setVisible(true);

    placeStatusMsg();
}

void MainWindow::placeStatusMsg()
{
    QRect scrRect{m_pScrollKkr->geometry()};
    QSize ms{m_pFloatingMsg->size()};
    QPoint pos{scrRect.x()+(scrRect.width()-ms.width())/2,
                scrRect.y()+(scrRect.height()-ms.height())/2};
    qDebug() << scrRect << ' ' << ms.width() << ',' << ms.height() << pos;
   //QPoint pos{scrRect.x(), scrRect.y()};
    m_pFloatingMsg->move(pos);
 }

/*
 * QWidget overrides
 */
bool MainWindow::eventFilter(QObject *pWatched, QEvent *e)
{
    if(pWatched == m_pScrollKkr) {
        switch(e->type()) {
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
            return true;
        default:
            break;
        }
    }

    return false;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    const auto st = m_ps.status();
    bool accept = true;

    if(st == ps::Status::INPLAY || st == ps::Status::PAUSED) {
        accept = (
            QMessageBox::Yes
                    == QMessageBox::question(this,
                                             tr("Kakuro"),
                                             tr("Play in progress. Really want to quit?"))
        );
    }

    if(accept)
        e->accept();
    else
        e->ignore();
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    if(m_pFloatingMsg->isEnabled())
        placeStatusMsg();
}

/*
 * slots
 */
void MainWindow::open()
{
    const auto st = m_ps.status();
    if(st == ps::Status::INPLAY || st == ps::Status::PAUSED) {
        const auto reply
                = QMessageBox::question(this,
                                        tr("Kakuro"),
                                        tr("Play in progress. Really want to open a new one?"));
        if(reply != QMessageBox::Yes)
            return;
    }

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
        showStatusMsg();
        m_pButtonPlay->setText(sStartB);
        m_pButtonPlay->setEnabled(true);
        m_pButtonCheck->setEnabled(false);
        m_pActionPlay->setText(sStartM);
        m_pActionPlay->setShortcut(Qt::Key_S | Qt::ControlModifier);
        m_pActionPlay->setEnabled(true);
        m_pActionCheck->setEnabled(false);
        m_pActionGiveup->setEnabled(false);
        m_secTimer.stop();
        setTimeIndicator(true);
        break;
    case ps::Status::INPLAY:
        showStatusMsg();
        m_pButtonPlay->setText(sPauseB);
        m_pButtonCheck->setEnabled(true);
        m_pActionPlay->setText(sPauseM);
        m_pActionPlay->setShortcut(Qt::Key_P | Qt::ControlModifier);
        m_pActionCheck->setEnabled(true);
        m_pActionGiveup->setEnabled(true);
        setTimeIndicator();
        m_secTimer.start(TIMER_INTERVAL);
        m_pKkrBoard->setFocus();
        break;
    case ps::Status::PAUSED:
        showStatusMsg(tr("Paused"));
        m_pButtonPlay->setText(sResumeB);
        m_pButtonCheck->setEnabled(false);
        m_pActionPlay->setText(sResumeM);
        m_pActionPlay->setShortcut(Qt::Key_R | Qt::ControlModifier);
        m_pActionCheck->setEnabled(false);
        m_secTimer.stop();
        break;
    case ps::Status::DONE:
        if(m_ps.isSolved())
            showStatusMsg(tr("Solved"));
        else
            showStatusMsg(tr("Given Up"));
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

void MainWindow::checkIt()
{
    if(m_uam.isSolved()) {
        emit solved();
        QMessageBox::information(this, tr("Solved!"), tr("Congratulations!"));
    } else {
        QMessageBox::warning(this, tr("Sorry"), tr("The answer is wrong"));
    }
}

void MainWindow::makeSureGiveup()
{
    const auto ans = QMessageBox::question(this, tr("Kakuro"), tr("Give up? Really!?"));
    if(ans == QMessageBox::Yes)
        emit giveup();
}
