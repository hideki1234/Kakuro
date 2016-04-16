#ifndef KKRBOARD_H
#define KKRBOARD_H

#include <QWidget>
#include <QFont>
#include <QPoint>
#include <QMouseEvent>
#include <QScrollArea>
#include "cosmetic.h"
#include "problemdata.h"
#include "playstatus.h"
#include "useranswermanager.h"
#include <memory>

namespace pd = problemdata;
namespace ua = useranswer;

class KkrBoard : public QWidget
{
    Q_OBJECT
    // dimensions
    int m_inner_width;
    int m_inner_height;
    int m_frame_width;
    int m_frame_height;
    int m_board_width;
    int m_board_height;

    // fonts for digits
    QFont m_fontAns;
    QFont m_fontClue;

    // UI flags
    bool m_showDigits;
    bool m_acceptInput;

    // data
    std::shared_ptr<pd::ProblemData> m_pData;
    ua::SharedAnswer m_pAns;

    // user input sub window
    int m_inCol;
    int m_inRow;
    int m_inValue;
    QWidget *m_pCellInput;

    // input cursor
    int m_curCol;
    int m_curRow;
    // scroll cell position
    int m_scrCol;
    int m_scrRow;

    // for auto scroll
    QScrollArea *m_pScrollArea;

    /*
     * coord conversion
     */

    // cell coord -> widget coord
    QRect getCellRect(int col, int row) const;
    QRect getCellRect(const QPoint &pt) const {return getCellRect(pt.x(), pt.y());}
    QRect getClueRectRight(const QRect &cellRect) const;
    QRect getClueRectDown(const QRect &cellRect) const;

    // widget coord -> cell coord
    QPoint getCellCoord(int x, int y) const;
        // if widget coord is not on a cell, returns (-1, -1)

    /*
     * paint sub methods
     */
    void drawCell(QPainter &p, int col, int row) const;
    void showCell(int col, int row);
    void resetCursor(int newCol, int newRow);

    /*
     * key inputs
     */
    void keyCursor(QKeyEvent *e);
    void keyData(QKeyEvent *e);

public:
    explicit KkrBoard(QWidget *parent = 0);
    ~KkrBoard();

    void setScrollArea(QScrollArea *pScrollArea);
    int getCurrentCellValue() const {return m_inValue;}

signals:
    void newAnswerInput(ua::CellData cellData);

public slots:
    void updateProblem(std::shared_ptr<pd::ProblemData> pNewData);
    void updateStatus(playstatus::Status newStatus);
    void updateUserAnswer(ua::SharedAnswer pNewAns);
    void renderAnswer(QPoint cellPos);

    // from cell input
    void cellInput(int value);

protected:
    /*
     * QWidget overrides
     */
    void paintEvent(QPaintEvent * e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
};

#endif // KKRBOARD_H
