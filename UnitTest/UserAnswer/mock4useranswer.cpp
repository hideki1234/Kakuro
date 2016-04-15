#include "../../Kakuro/problemdata.h"
#include <QStringList>

namespace problemdata {

class ProblemData_int {
public:
    int cols;
    int rows;
};

ProblemData::ProblemData(std::unique_ptr<ProblemData_int> m) : m_(std::move(m))
{
}

ProblemData::~ProblemData()
{
}

int ProblemData::getNumCols() const
{
    return m_->cols;
}

int ProblemData::getNumRows() const
{
    return m_->rows;
}

CellType ProblemData::getCellType(int col, int row) const
{
    return (col+row)%2 == 1 ? CellType::CellAnswer : CellType::CellClue;
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

ProblemData *ProblemData::problemLoader(const QString &filename)
{
    ProblemData * pPd = new ProblemData(std::unique_ptr<ProblemData_int>(new ProblemData_int));
    QStringList l = filename.split(',');
    pPd->m_->cols = l[0].toInt();
    pPd->m_->rows = l[1].toInt();

    return pPd;
}

}
