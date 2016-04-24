#ifndef KKRBOARDMANAGER_H
#define KKRBOARDMANAGER_H

#include <QObject>
#include <vector>

enum class CellType {
    CellAnswer,
    CellClue
};

const static int CLOSED_CLUE = 0;
const static int NO_CLUE = 0;
const static int NO_ANSWER = 0;

class BoardData {
    // internal data
    struct Cell {
        CellType ctype;
        union {
            int answer;
            struct {
                int clueRight;
                int clueDown;
            };
        };
    };
    const int m_cols;
    const int m_rows;
    std::vector<Cell> m_data;

    // col,row to index
    int c2i(int c, int r) const {return r*m_cols + c;}

public:
    BoardData(int cols, int rows);
    ~BoardData();

    BoardData(const BoardData &) = delete;
    BoardData &operator=(const BoardData&) = delete;

    int getNumCols() const {return m_cols;}
    int getNumRows() const {return m_rows;}

    CellType getCellType(int col, int row) const;
    void setCellType(int col, int row, CellType ct);

    int getClueRight(int col, int row) const;
    void setClueRight(int col, int row, int clue);

    int getClueDown(int col, int row) const;
    void setClueDown(int col, int row, int clue);

    int getAnswer(int col, int row) const;
    void setAnswer(int col, int row, int ans);
};

class KkrBoardManager : public QObject
{
    Q_OBJECT
public:
    explicit KkrBoardManager(QObject *parent = 0);

signals:

public slots:
    void slCreate(int cols, int rows);
};

#endif // KKRBOARDMANAGER_H
