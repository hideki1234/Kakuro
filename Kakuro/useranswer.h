#ifndef USERANSWER_H
#define USERANSWER_H

namespace useranswer {

class UserAnswer
{
    // only friend class UserAnswerManger can construct me
    UserAnswer();

public:
    ~UserAnswer();

    UserAnswer(const UserAnswer&) = delete;
    UserAnswer &operator=(const UserAnswer&) = delete;

    int getNumCols() const;
    int getNumRows() const;
    int getAnswer(int col, int row) const;

    friend class UserAnswerManager;
};

}   // namespace useranswer

#endif // USERANSWER_H
