#include "celldelegate.h"

#include <QPainter>
#include <QDebug>

#include "busman.h"
#include "busmantablemodel.h"


CellDelegate::CellDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}


void CellDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 viewOption = option;

    painter->save();

    QBrush brush = viewOption.backgroundBrush;
    QColor textColor = Qt::black;
    QRect rect = viewOption.rect;

//    QString textCell = index.model()->data(index, Qt::DisplayRole).toString();
    Busman* busman = index.model()->data(index, BusmanTableModel::BusmanRole).value<Busman*>();
    QString textCell = index.model()->data(index, BusmanTableModel::WishDayRole).toString();
//    if (busman == 0) {
//        return;
//    }

//    QString textCell = busman->wishesOnSchedule.at(index.)

//    qDebug() << "Value:" << textCell << rect.size().width() << busman;
    if (textCell == "RR") {
        brush = QBrush(Qt::lightGray);

    } else if (textCell == "XX") {
        textColor = Qt::white;
        brush = QBrush(Qt::black);

// TODO: градиент как скрине
//        QLinearGradient gradient(rect.topLeft(), rect.bottomRight());
//        gradient.setSpread(QGradient::RepeatSpread);
//        gradient.setColorAt(0, Qt::black);
//        gradient.setColorAt(1, Qt::white);
//        brush = QBrush(gradient);

    } else if (textCell == "DD") {
        brush = QBrush(Qt::yellow);
        rect.setWidth(rect.width() / 2);

    } else if (textCell == "NN") {
        brush = QBrush(Qt::yellow);
        rect.setX(rect.x() + rect.width() / 2);
    }

    painter->fillRect(rect, brush);

//    if (busman) {
//        // TODO: Номер автобуса + Выбор линии маршрута + количесто дней в расписании
//        int dayNumber = index.column() - 2;
//        qDebug() << "dayNumber: " << dayNumber;

//        // Если день у водителя выбран
//        if (busman->workingDays.contains(dayNumber)) {
//            Busman::DayKind dayKind = busman->workingDays[dayNumber];
//            switch (dayKind) {
//                case Busman::DayKind::LINE_1_DAY:
//                case Busman::DayKind::LINE_1_NIGHT:
//                case Busman::DayKind::LINE_2_DAY:
//                case Busman::DayKind::LINE_2_NIGHT:
//                case Busman::DayKind::LINE_3_DAY:
//                case Busman::DayKind::LINE_3_NIGHT:
//                    break;
//            }
//        }
//    }

//    if (value != "00") {
//        painter->setPen(QPen(textColor));
//        QRect rect = viewOption.rect;
//        Qt::Alignment alignText = Qt::AlignCenter;
//        painter->drawText(rect, alignText, value);
//    }

// TODO: непонятно как иначе нарисовать текст -- нужно указать цвет текста
// и при ручной отрисовке с этим проблем нет, но как сделать стандартными средствами
//    painter->setPen(QPen(textColor));
//    QRect rect = viewOption.rect;
//    Qt::Alignment alignText = Qt::AlignCenter;
//    painter->drawText(rect, alignText, value);

    painter->restore();

    // Здесь дорисовываются стандартные вещи вроде текста, которые берутся из модели
    QStyledItemDelegate::paint( painter, option, index );
}

//QColor& QSomeColumnDelegate::MixColors(const QColor ColorA, const QColor ColorB) const{
//    QColor result = QColor(ColorA.rgba());
//    result.setRed(   (result.red() + ColorB.red() )    / 2);
//    result.setGreen( (result.green() + ColorB.green() )/ 2);
//    result.setBlue(  (result.blue() + ColorB.blue() )  / 2);
//    result.setAlpha(30);

//    return result;
//}
