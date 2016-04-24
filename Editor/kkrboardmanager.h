#ifndef KKRBOARDMANAGER_H
#define KKRBOARDMANAGER_H

#include <QObject>

enum class CellType {
    CellAnswer,
    CellClue
};

const static int CLOSED_CLUE = 0;
const static int NO_CLUE = 0;
const static int NO_ANSWER = 0;

class BoardData {

public:
    BoardData(int cols, int rows);
    ~BoardData();

    BoardData(const BoardData &) = delete;
    BoardData &operator=(const BoardData&) = delete;

    int getNumCols() const;
    int getNumRows() const;

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
