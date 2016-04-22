#ifndef KKREDITMAIN_H
#define KKREDITMAIN_H

#include <QMainWindow>
#include <QMenu>
#include <QScrollArea>
#include "kkrworkboard.h"
#include "metadatamanager.h"
#include "metadataview.h"

class KkrEditMain : public QMainWindow
{
    Q_OBJECT

    // main board
    QScrollArea *m_pScrollBoard;
    KkrWorkBoard *m_pWorkBoard;

    // meta data view
    MetaDataView *m_pMetaView;

    // Menu
    QMenu *m_pMenuView;

    /*
     * models
     */
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
};

#endif // KKREDITMAIN_H
