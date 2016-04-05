#include <memory>
#include "problemdata.h"
#include <QtGlobal>

namespace problemdata {

ProblemData::ProblemData()
{

}

ProblemData::~ProblemData()
{

}

CellType ProblemData::getCellType(int col, int row) const
{
    return m_data[cr2i(col,row)].type;
}

int ProblemData::getClueRight(int col, int row) const
{
    const auto i = cr2i(col, row);
    Q_ASSERT(m_data[i].type == CellType::CellClue);

    return m_data[i].right;
}

int ProblemData::getClueDown(int col, int row) const
{
    const auto i = cr2i(col, row);
    Q_ASSERT(m_data[i].type == CellType::CellClue);

    return m_data[i].down;
}

int ProblemData::getAnswer(int col, int row) const
{
    const auto i = cr2i(col, row);
    Q_ASSERT(m_data[i].type == CellType::CellAnswer);

    return m_data[i].ans;
}

ProblemData *ProblemData::problemLoader(const QString &filename)
{
    std::unique_ptr<ProblemData> pNewData{new ProblemData};
    // TODO: need to replace with actual code
    // set up dummy data
    pNewData->m_cols = 3;
    pNewData->m_rows = 3;

    auto &data = pNewData->m_data;
    data.reserve(pNewData->m_cols * pNewData->m_rows);

    auto i = pNewData->cr2i(0,0);
    data[i].type = CellType::CellClue;
    data[i].right = data[i].down = 0;

    i = pNewData->cr2i(1,0);
    data[i].type = CellType::CellClue;
    data[i].right = 0;
    data[i].down = 4;

    i = pNewData->cr2i(2,0);
    data[i].type = CellType::CellClue;
    data[i].right = 0;
    data[i].down = 7;

    i = pNewData->cr2i(0,1);
    data[i].type = CellType::CellClue;
    data[i].right = 3;
    data[i].down = 0;

    i = pNewData->cr2i(1,1);
    data[i].type = CellType::CellAnswer;
    data[i].ans = 1;

    i = pNewData->cr2i(2,1);
    data[i].type = CellType::CellAnswer;
    data[i].ans = 2;

    i = pNewData->cr2i(0,2);
    data[i].type = CellType::CellClue;
    data[i].right = 8;
    data[i].down = 0;

    i = pNewData->cr2i(1,2);
    data[i].type = CellType::CellAnswer;
    data[i].ans = 3;

    i = pNewData->cr2i(1,2);
    data[i].type = CellType::CellAnswer;
    data[i].ans = 5;

    return pNewData.release();
}

}	// namespace problemdata
