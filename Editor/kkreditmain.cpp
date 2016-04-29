#include <QBoxLayout>
#include "kkreditmain.h"
#include "metadataview.h"
#include <QMenuBar>
#include <QDockWidget>
#include <QMessageBox>
#include <QEvent>
#include "dialognew.h"

void KkrEditMain::setupMainMenu()
{
    QMenuBar *pMainMenu = new QMenuBar;

    // File menu
    QMenu *pMenuFile = new QMenu{tr("&File")};
    pMenuFile->addAction(tr("&New..."), this, &KkrEditMain::newWorkBoard);
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
    QPalette palBack(palette());
    palBack.setColor(QPalette::Background, Qt::white);
    m_pScrollBoard->setPalette(palBack);
    m_pScrollBoard->setAutoFillBackground(true);
    m_pWorkBoard = new KkrBoardView(&m_BoardData);
    m_pWorkBoard->setScrollArea(m_pScrollBoard);
    m_pScrollBoard->setWidget(m_pWorkBoard);
    m_pScrollBoard->installEventFilter(this);

    m_pMetaView = new MetaDataView(&m_MetaData, this);
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

    // connections
    connect(this, &KkrEditMain::sigNewMeta, &m_MetaData, &MetaDataManager::slCreate);
    connect(this, &KkrEditMain::sigNewBoard, &m_BoardData, &KkrBoardManager::slCreate);
}

KkrEditMain::~KkrEditMain()
{

}

/*
 * Menu actions
 */
void KkrEditMain::newWorkBoard()
{
    if(m_pWorkBoard->isEdited()) {
        const auto bSave = QMessageBox::question(this, tr("Kakuro Editor"),
                                                 tr("Problem is updated. Save?"));
        if(bSave == QMessageBox::Yes)
            saveWorkBoard();
    }

    DialogNew dlgNew{this};
    if(dlgNew.exec() == QDialog::Rejected)
        return;

    emit sigNewMeta();
    // UI size excludes topmost clue-only row and leftmost clue-only column
    // Internal data includes them
    emit sigNewBoard(dlgNew.getNumCols()+1, dlgNew.getNumRows()+1);

    m_pWorkBoard->setFocus();
}

void KkrEditMain::saveWorkBoard()
{
    // TODO
    QMessageBox::information(this, tr("kakuro Editor"), "Not implemented yet");
}

/*
 * event filter
 */
bool KkrEditMain::eventFilter(QObject *pWatched, QEvent *e)
{
    if(pWatched == m_pScrollBoard) {
        switch(e->type()) {
        case QEvent::FocusIn:
            m_pWorkBoard->setFocus();
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
            return true;
        default:
            break;
        }
    }
    return false;
}
