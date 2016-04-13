#ifndef USERANSWER_H
#define USERANSWER_H

#include <vector>

namespace useranswer {

static const int ANSWER_NODATA = 0;

class UserAnswer
{
    const int m_numCols;
    const int m_numRows;
    std::vector<int> m_answers;

    // only friend class UserAnswerManger can construct me
    UserAnswer(int numCols, int numRows);
    int getIndex(int col, int row) const {return col + row * m_numCols;}

public:
    ~UserAnswer();

    UserAnswer(const UserAnswer&) = delete;
    UserAnswer &operator=(const UserAnswer&) = delete;

    int getNumCols() const {return m_numCols;}
    int getNumRows() const {return m_numRows;}
    int getAnswer(int col, int row) const {return m_answers[getIndex(col,row)];}

    friend class UserAnswerManager;
};

}   // namespace useranswer

#endif // USERANSWER_H
