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

/*
 * functions for data loader
 */

static const int INVALID_DATA = -1;

static int chars2int(const char *buffer, int len)
{
    int val = 0;
    for(int i = 0; i < len; ++i) {
        if(!std::isdigit(buffer[i]))
            return INVALID_DATA;
        val *= 10;
        val += buffer[i] - '0';
    }
    return val;
}

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

    return chars2int(buffer, VERSION_SIZE);
}

static const int SIZE_LEN = 4;
static const int VER0_TYPE_LEN = 1;
static const char VER0_CELL_ANSWER = '0';
static const char VER0_CELL_CLUE = '1';
static const int VER0_ANS_LEN = 1;
static const int VER0_CLUE_LEN = 2;

ProblemData *ProblemData::problemLoader(const QString &filename)
{
    QFile f_data{filename};
    if(!f_data.open(QIODevice::ReadOnly))
        return nullptr;

    if(parseHeader(f_data) != 0)
        return nullptr;

    std::unique_ptr<ProblemData> pNewData{new ProblemData};
    char buffer[4];
    int byteRead;

    byteRead = f_data.read(buffer, SIZE_LEN);
    if(byteRead != SIZE_LEN)
        return nullptr;
    pNewData->m_cols = chars2int(buffer, SIZE_LEN);
    if(pNewData->m_cols == INVALID_DATA)
        return nullptr;

    byteRead = f_data.read(buffer, SIZE_LEN);
    if(byteRead != SIZE_LEN)
        return nullptr;
    pNewData->m_rows = chars2int(buffer, SIZE_LEN);
    if(pNewData->m_rows == INVALID_DATA)
        return nullptr;

    auto &data = pNewData->m_data;
    data.resize(pNewData->m_cols * pNewData->m_rows);
    for(int r = 0; r < pNewData->m_rows; ++r) {
        for(int c = 0; c < pNewData->m_cols; ++c) {
            const auto i = pNewData->cr2i(c,r);

            // cell type
            byteRead = f_data.read(buffer, VER0_TYPE_LEN);
            if(byteRead != VER0_TYPE_LEN)
                return nullptr;
            switch(buffer[0]) {
            case VER0_CELL_ANSWER:
                data[i].type = CellType::CellAnswer;
                byteRead = f_data.read(buffer, VER0_ANS_LEN);
                if(byteRead != VER0_ANS_LEN)
                    return nullptr;
                data[i].ans = chars2int(buffer, VER0_ANS_LEN);
                if(data[i].ans < 1 || 9 < data[i].ans)
                    return nullptr;
                break;
            case VER0_CELL_CLUE:
                data[i].type = CellType::CellClue;

                // right
                byteRead = f_data.read(buffer, VER0_CLUE_LEN);
                if(byteRead != VER0_CLUE_LEN)
                    return nullptr;
                data[i].right = chars2int(buffer, VER0_CLUE_LEN);
                if(data[i].right < 0 || 45 < data[i].right)
                    return nullptr;

                // down
                byteRead = f_data.read(buffer, VER0_CLUE_LEN);
                if(byteRead != VER0_CLUE_LEN)
                    return nullptr;
                data[i].down = chars2int(buffer, VER0_CLUE_LEN);
                if(data[i].down < 0 || 45 < data[i].down)
                    return nullptr;
                break;
            default:
                return nullptr;
            }
        }
    }

    return pNewData.release();
}

}	// namespace problemdata
