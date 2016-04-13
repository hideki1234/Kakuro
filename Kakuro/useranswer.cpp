#include "useranswer.h"

namespace useranswer {

UserAnswer::UserAnswer(int numCols, int numRows)
    : m_numCols(numCols)
    , m_numRows(numRows)
    , m_answers(numCols*numRows, ANSWER_NODATA)
{
}

UserAnswer::~UserAnswer()
{
    // nothing to do
}

}   // namespace useranswer
