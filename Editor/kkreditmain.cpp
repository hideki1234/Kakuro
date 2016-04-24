#include <QBoxLayout>
#include "kkreditmain.h"
#include "metadataview.h"
#include <QMenuBar>
#include <QDockWidget>

void KkrEditMain::setupMainMenu()
{
    QMenuBar *pMainMenu = new QMenuBar;

    // File menu
    QMenu *pMenuFile = new QMenu{tr("&File")};
    pMenuFile->addAction(tr("E&xit"), this, &QWidget::close);
    pMainMenu->addMenu(pMenuFile);

    // View menu
    m_pMenuView = new QMenu{tr("&View")};
    pMainMenu->addMenu(m_pMenuView);

    setMenuBar(pMainMenu);
}

void KkrEditMain::makeCoreWidgets()
{
    m_pScrollBoard = new QScrollArea;
    m_pWorkBoard = new KkrBoardView(m_pScrollBoard);
    QPalette palBack(palette());
    palBack.setColor(QPalette::Background, Qt::white);
    m_pScrollBoard->setPalette(palBack);
    m_pScrollBoard->setAutoFillBackground(true);
    m_pWorkBoard->setScrollArea(m_pScrollBoard);

    m_pMetaView = new MetaDataView(&m_MetaData);
 }

void KkrEditMain::setupCentralPane()
{
    QWidget *pWidget = new QWidget;
    QBoxLayout *pBL = new QBoxLayout(QBoxLayout::TopToBottom, pWidget);

    pBL->addWidget(m_pScrollBoard);

    setCentralWidget(pWidget);
}

void KkrEditMain::setupDocks()
{
    QDockWidget *pDockMetaData = new QDockWidget{tr("Problem Info"), this};
    pDockMetaData->setWidget(m_pMetaView);
    pDockMetaData->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_pMenuView->addAction(pDockMetaData->toggleViewAction());
    addDockWidget(Qt::LeftDockWidgetArea, pDockMetaData);
}

KkrEditMain::KkrEditMain(QWidget *parent)
    : QMainWindow(parent)
{
    makeCoreWidgets();
    setupCentralPane();
    setupMainMenu();
    setupDocks();
}

KkrEditMain::~KkrEditMain()
{

}
