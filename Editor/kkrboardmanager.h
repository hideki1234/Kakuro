#ifndef KKRBOARDMANAGER_H
#define KKRBOARDMANAGER_H

#include <QObject>
#include <vector>
#include <memory>

enum class CellType {
    CellAnswer,
    CellClue
};

const static int CLOSED_CLUE = -1;
const static int EMPTY_CLUE = 0;
const static int EMPTY_ANSWER = 0;

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

class BoardData {
    const int m_cols;
    const int m_rows;
    std::vector<Cell> m_data;

    // col,row to index
    int c2i(int c, int r) const {return r*m_cols + c;}

public:
    BoardData(int cols, int rows);
    ~BoardData();
    BoardData(const BoardData &rhs);

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

friend class KkrBoardManager;
};

class KkrBoardManager : public QObject
{
    Q_OBJECT

    int m_cols;
    int m_rows;
    std::vector<Cell> m_data;

    // col,row to index
    int c2i(int c, int r) const {return r*m_cols + c;}

    void setCellType(int col, int row, CellType ct);
    void setClueRight(int col, int row, int clue);
    void setClueDown(int col, int row, int clue);
    void setAnswer(int col, int row, int ans);

public:
    explicit KkrBoardManager(QObject *parent = 0);

    int getNumCols() const {return m_cols;}
    int getNumRows() const {return m_rows;}

    CellType getCellType(int col, int row) const;
    int getClueRight(int col, int row) const;
    int getClueDown(int col, int row) const;
    int getAnswer(int col, int row) const;

    bool isValid() const;

    std::unique_ptr<BoardData> getBoardData() const;

signals:
    void sigReset();

public slots:
    void slCreate(int cols, int rows);
    void slRead(std::shared_ptr<const BoardData> data);

friend class KkrBoardView;
};

#endif // KKRBOARDMANAGER_H
