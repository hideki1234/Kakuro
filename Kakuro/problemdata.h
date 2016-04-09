#ifndef PROBLEMDATA_H
#define PROBLEMDATA_H

#include <QString>
#include <memory>

namespace problemdata {

enum class CellType {
    CellAnswer,
    CellClue
};

const static int CLOSED_CLUE = 0;

class ProblemData_int;

class ProblemData
{
    std::unique_ptr<ProblemData_int> m_;

    // ctor - only accessible from factory method
    ProblemData(std::unique_ptr<ProblemData_int> m);

public:
    ~ProblemData();

    int getNumCols() const;
        // returns # of columns; includes leftmost clue only columns
    int getNumRows() const;
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

    // delete unnecessary default methods to make this immutable
    ProblemData(const ProblemData&) = delete;
    ProblemData & operator=(const ProblemData&) = delete;
};

}	// namespace problemdata

#endif // PROBLEMDATA_H
