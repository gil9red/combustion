#ifndef LINEDAYS_CELLDELEGATE_H
#define LINEDAYS_CELLDELEGATE_H


#include <QStyledItemDelegate>


class LineDays_CellDelegate : public QStyledItemDelegate
{
    public:
        LineDays_CellDelegate(QWidget* parent=nullptr);
//        LineDays_CellDelegate(QWidget* parent=nullptr): QStyledItemDelegate() {
//            this->parentTable = parent;
//        }

    public:
//            enum SelectedSide {
//                None,
//                Left,
//                Right,
//            };
//            SelectedSide side = SelectedSide::None;

        QWidget* parentTable;

    protected:
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};



#endif // LINEDAYS_CELLDELEGATE_H
