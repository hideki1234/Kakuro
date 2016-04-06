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

void MainWindow::makeCoreWidgets()
{
    m_pButtonPlay = new QPushButton{tr("Play")};
    m_pButtonPlay->setMinimumWidth(BUTTON_WIDTH);
    m_pButtonUndo = new QPushButton{tr("Undo")};
    m_pButtonUndo->setMinimumWidth(BUTTON_WIDTH);
    m_pButtonCheck = new QPushButton{tr("Check")};
    m_pButtonCheck->setMinimumWidth(BUTTON_WIDTH);

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

    setMinimumWidth(MAIN_WIDTH);
    setMinimumHeight(MAIN_HEIGHT);
}

MainWindow::~MainWindow()
{

}

namespace pd = problemdata;

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

    pd::ProblemData * pData = pd::ProblemData::problemLoader(filename);
    if(pData == nullptr) {
        QMessageBox::critical(this, tr("Kakuro Player"), tr("Failed to open ") + filename);
        return;
    }


}
