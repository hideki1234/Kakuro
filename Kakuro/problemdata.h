#ifndef PROBLEMDATA_H
#define PROBLEMDATA_H

#include <QString>
#include <vector>

namespace problemdata {

enum class CellType {
    CellAnswer,
    CellClue
};

const static int CLOSED_CLUE = 0;

class ProblemData
{
    struct Cell {
        CellType type;
        union {
            char ans;
            struct {
                char right;
                char down;
            };
        };
    };

    int m_cols;
    int m_rows;
    std::vector<Cell> m_data;

    // ctor - only accessible from factory method
    ProblemData();

    int cr2i(int c, int r) const {return r * m_cols + c;}
        // calculate index of m_data from col and row

public:
    ~ProblemData();

    int getNumCols() const {return m_cols;}
        // returns # of columns; includes leftmost clue only columns
    int getNumRows() const {return m_rows;}
        // return # of rows; includes topmost clue only rows
    CellType getCellType(int col, int row) const;
        // returns the cell type of specified cell
    int getClueRight(int col, int row) const;
    int getClueDown(int col, int row) const;
        // get clue; only valid for clue cells
    int getAnswer(int col, int row) const;
        // return answer of cell; only valid for answer cells

    static ProblemData *problemLoader(const QString &filename);
        // factory method; load problem data from a file
};

}	// namespace problemdata

#endif // PROBLEMDATA_H
