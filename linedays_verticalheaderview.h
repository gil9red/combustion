#ifndef LINEDAYS_VERTICALHEADERVIEW_H
#define LINEDAYS_VERTICALHEADERVIEW_H


#include <QHeaderView>


class LineDays_VerticalHeaderView: public QHeaderView
{
    Q_OBJECT

    public:
        LineDays_VerticalHeaderView();

    protected:
        void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const;
};


#endif // LINEDAYS_VERTICALHEADERVIEW_H
