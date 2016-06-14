#ifndef LINEDAYSTABLE_H
#define LINEDAYSTABLE_H


#include <QTableView>
#include "linedaystablemodel.h"
#include <QDebug>
#include <QMouseEvent>
#include <QFocusEvent>


class LineDaysTable : public QTableView
{
    Q_OBJECT

    public:
        LineDaysTable();

        // TODO:
        LineDaysTableModel model;

//        // TODO
//        QPoint clickedPos = QPoint(-1, -1);

//    protected:
//        void mousePressEvent(QMouseEvent* event) {
//            qDebug() << event->pos() << viewport()->mapFromGlobal(QCursor::pos());
//            clickedPos = event->pos();

//            QTableView::mousePressEvent(event);
//        }

//        void mouseReleaseEvent(QMouseEvent* event) {
//            qDebug() << event->pos() << viewport()->mapFromGlobal(QCursor::pos());
//            clickedPos = event->pos();

//            QTableView::mouseReleaseEvent(event);
//        }

//        void focusOutEvent(QFocusEvent* event) {
////            clickedPos = QPoint(-1, -1);
//            qDebug() << "focusOutEvent";

//            QTableView::focusOutEvent(event);
//        }

//    private slots:
//        void clickedIndex(const QModelIndex& index) {
//            clickedPos = viewport()->mapFromGlobal(QCursor::pos());
//            qDebug() << index << clickedPos;
//        }
};


#endif // LINEDAYSTABLE_H
