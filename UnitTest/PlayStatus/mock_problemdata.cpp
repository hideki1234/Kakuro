#include "../../Kakuro/problemdata.h"

namespace problemdata {

class ProblemData_int {
};

ProblemData::ProblemData(std::unique_ptr<ProblemData_int> m) : m_(std::move(m))
{
}

ProblemData::~ProblemData()
{
}

int ProblemData::getNumCols() const
{
    return 9;
}

int ProblemData::getNumRows() const
{
    return 3;
}

CellType ProblemData::getCellType(int col, int row) const
{
    return (col+row)%2 == 0 ? CellType::CellAnswer : CellType::CellClue;
}

int ProblemData::getClueRight(int col, int row) const
{
    return (col+row)%43 + 3;
}

int ProblemData::getClueDown(int col, int row) const
{
    return 45 - (col+row)%43;
}

int ProblemData::getAnswer(int col, int row) const
{
    return (col+row)%9 + 1;
}

ProblemData *ProblemData::problemLoader(const QString &)
{
    return new ProblemData(nullptr);
}

}
