#ifndef KKRBOARD_H
#define KKRBOARD_H

#include <QWidget>
#include <QFont>
#include "problemdata.h"
#include "playstatus.h"
#include <memory>

namespace pd = problemdata;

class KkrBoard : public QWidget
{
    Q_OBJECT
    // dimensions
    static const int MARGIN = 10;
    static const int FRAME_THICK = 5;
    static const int BORDER_THICK = 1;
    static const int CELL_WIDTH = 36;
    static const int CLUE_WIDTH = CELL_WIDTH/2 - 4;

    int m_inner_width;
    int m_inner_height;
    int m_frame_width;
    int m_frame_height;
    int m_board_width;
    int m_board_height;

    // fonts for digits
    QFont m_fontAns;
    QFont m_fontClue;

    // paint flags
    bool m_showDigits;

    std::shared_ptr<pd::ProblemData> m_pData;

    QRect getCellRect(int col, int row) const;
    QRect getClueRectRight(const QRect &cellRect) const;
    QRect getClueRectDown(const QRect &cellRect) const;

    // paint sub methods
    void drawCell(QPainter &p, int col, int row) const;

public:
    explicit KkrBoard(QWidget *parent = 0);

signals:

public slots:
    void updateData(std::shared_ptr<pd::ProblemData> pNewData);
    void updateStatus(playstatus::Status newStatus);

protected:
    void paintEvent(QPaintEvent * e) override;
};

#endif // KKRBOARD_H
