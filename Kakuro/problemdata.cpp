#include <memory>
#include <cstring>
#include <cctype>
#include "problemdata.h"
#include <QFile>
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

static const int INVALID_DATA = -1;
static int parseHeader(QFile &f_data)
// parse the header of kakuro data and returns the version (>=0)
// if the file is invalid, return -1 (INVALID_DATA)
{
    static char HEADER[] = {'K', 'K', 'R', 'P'};
    const static unsigned VERSION_SIZE = 4;

    char buffer[4];
    qint64 byteRead;

    // header check
    byteRead = f_data.read(buffer, sizeof(HEADER));
    if(byteRead != sizeof(HEADER))
        return INVALID_DATA;
    if(std::memcmp(HEADER, buffer, sizeof(HEADER)) != 0)
        return INVALID_DATA;

    // version check
    byteRead = f_data.read(buffer, VERSION_SIZE);
    if(byteRead != VERSION_SIZE)
        return INVALID_DATA;
    int version = 0;
    for(auto i = 0u; i < VERSION_SIZE; ++i) {
        if(!std::isdigit(buffer[i]))
            return INVALID_DATA;
        version *= 10;
        version += buffer[i] - '0';
    }

    return version;
}

ProblemData *ProblemData::problemLoader(const QString &filename)
{
    QFile f_data{filename};
    if(!f_data.open(QIODevice::ReadOnly))
        return nullptr;

    if(parseHeader(f_data) != 0)
        return nullptr;

    std::unique_ptr<ProblemData> pNewData{new ProblemData};
    // TODO: need to replace with actual code
    // set up dummy data
    pNewData->m_cols = 3;
    pNewData->m_rows = 3;

    auto &data = pNewData->m_data;
    //data.reserve(pNewData->m_cols * pNewData->m_rows);
    data.resize(pNewData->m_cols * pNewData->m_rows);

    auto i = pNewData->cr2i(0,0);
    data[i].type = CellType::CellClue;
    data[i].right = data[i].down = CLOSED_CLUE;

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
