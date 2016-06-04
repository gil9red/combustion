#ifndef CELLDELEGATE_H
#define CELLDELEGATE_H


#include <QStyledItemDelegate>


class CellDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    public:
        explicit CellDelegate(QObject *parent = 0);

//    private:
//        //будем менять цвет смешиванием цветов
//        QColor& MixColors(const QColor ColorA, const QColor ColorB) const;

    protected:
        // чтобы управлять отрисовкой нужно переопределить этот метод
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif // CELLDELEGATE_H
