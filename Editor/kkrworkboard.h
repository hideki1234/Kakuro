#ifndef KKRWORKBOARD_H
#define KKRWORKBOARD_H

#include <QWidget>
#include <QScrollArea>
#include <QFont>
#include <QPainter>
#include "kkrboardmanager.h"

class KkrBoardView : public QWidget
{
    Q_OBJECT
    // fonts for digits
    QFont m_fontAns;
    QFont m_fontClue;

    // dimensions
    int m_inner_width;
    int m_inner_height;
    int m_frame_width;
    int m_frame_height;
    int m_board_width;
    int m_board_height;

    // input cursor
    int m_curCol;
    int m_curRow;
    enum class CursorClue {
        None, Down, Right,
    } m_curClue;

    // data
    KkrBoardManager *m_pBoardData;

    // scroll
    QScrollArea *m_pScrollArea;

    /*
     * coord conversion
     */
    // cell coord -> widget code
    QRect getCellRect(int col, int row) const;
    QRect getClueRectRight(const QRect &cellRect) const;
    QRect getClueRectDown(const QRect &cellRect) const;

    /*
     * drawing
     */
    void drawCell(QPainter &p, int col, int row) const;
    void showCell(int col, int row);
    void resetCursor(int col, int row, CursorClue cl = CursorClue::None);

    /*
     * key input
     */
    void keyCursor(QKeyEvent *e);
    void resetClueCell(int col, int row);
    void switchCellType(int col, int row);
    void keyValue(QKeyEvent *e);

public:
    explicit KkrBoardView(KkrBoardManager *m_pBoardData, QWidget *parent = 0);

    KkrBoardView(const KkrBoardView &) = delete;
    KkrBoardView &operator =(const KkrBoardView &) = delete;

    void setScrollArea(QScrollArea *pSA) {m_pScrollArea=pSA;}

    bool isEdited() const {/*TODO*/return false;}

protected:
    void paintEvent(QPaintEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

signals:

public slots:
    void slReset();
};

#endif // KKRWORKBOARD_H
