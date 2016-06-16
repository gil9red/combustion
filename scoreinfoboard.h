#ifndef SCOREINFOBOARD_H
#define SCOREINFOBOARD_H

#include <QWidget>

namespace Ui {
    class ScoreInfoBoard;
}

class ScoreInfoBoard : public QWidget
{
        Q_OBJECT

    public:
        explicit ScoreInfoBoard(QWidget *parent = 0);
        ~ScoreInfoBoard();

    private:
        Ui::ScoreInfoBoard *ui;
};

#endif // SCOREINFOBOARD_H
