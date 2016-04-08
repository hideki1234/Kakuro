#include "problemdata.h"
#include <vector>
#include <cstring>
#include <cctype>
#include <QFile>
#include <QtGlobal>

namespace problemdata {

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

class ProblemData_int {
public:
    int cols;
    int rows;
    std::vector<Cell> data;

    int cr2i(int c, int r) const {return r * cols + c;}
        // calculate index of m_data from col and row
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
    return m_->data[m_->cr2i(col,row)].type;
}

int ProblemData::getClueRight(int col, int row) const
{
    const auto i = m_->cr2i(col, row);
    Q_ASSERT(m_->data[i].type == CellType::CellClue);

    return m_->data[i].right;
}

int ProblemData::getClueDown(int col, int row) const
{
    const auto i = m_->cr2i(col, row);
    Q_ASSERT(m_->data[i].type == CellType::CellClue);

    return m_->data[i].down;
}

int ProblemData::getAnswer(int col, int row) const
{
    const auto i = m_->cr2i(col, row);
    Q_ASSERT(m_->data[i].type == CellType::CellAnswer);

    return m_->data[i].ans;
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

    std::unique_ptr<ProblemData_int> pNewData{new ProblemData_int};
    char buffer[4];
    int byteRead;

    byteRead = f_data.read(buffer, SIZE_LEN);
    if(byteRead != SIZE_LEN)
        return nullptr;
    pNewData->cols = chars2int(buffer, SIZE_LEN);
    if(pNewData->cols == INVALID_DATA)
        return nullptr;

    byteRead = f_data.read(buffer, SIZE_LEN);
    if(byteRead != SIZE_LEN)
        return nullptr;
    pNewData->rows = chars2int(buffer, SIZE_LEN);
    if(pNewData->rows == INVALID_DATA)
        return nullptr;

    auto &data = pNewData->data;
    data.resize(pNewData->cols * pNewData->rows);
    for(int r = 0; r < pNewData->rows; ++r) {
        for(int c = 0; c < pNewData->cols; ++c) {
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

    return new ProblemData(std::move(pNewData));
}

}	// namespace problemdata
