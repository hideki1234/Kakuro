#ifndef KKREDITMAIN_H
#define KKREDITMAIN_H

#include <QMainWindow>
#include <QMenu>
#include <QScrollArea>
#include "kkrworkboard.h"
#include "metadatamanager.h"
#include "metadataview.h"
#include "kkrboardmanager.h"
#include "kkrworkboard.h"

class KkrEditMain : public QMainWindow
{
    Q_OBJECT

    // main board
    QScrollArea *m_pScrollBoard;
    KkrBoardView *m_pWorkBoard;

    // meta data view
    MetaDataView *m_pMetaView;

    // Menu
    QMenu *m_pMenuView;

    /*
     * models
     */
    KkrBoardManager m_BoardData;
    MetaDataManager m_MetaData;

    /*
     * initialization
     */
    void setupMainMenu();
    void makeCoreWidgets();
    void setupCentralPane();
    void setupDocks();

public:
    KkrEditMain(QWidget *parent = 0);
    ~KkrEditMain();

    /*
     * Menu actions
     */
    void newWorkBoard();
    void saveWorkBoard();

signals:
    void sigNewMeta();
    void sigNewBoard(int cols, int rows);
};

#endif // KKREDITMAIN_H
