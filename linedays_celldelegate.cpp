#include "linedays_celldelegate.h"
#include "linedaystablemodel.h"

#include <QMouseEvent>
#include <QPainter>
#include <QDebug>


bool LineDays_CellDelegate::eventFilter(QObject* obj, QEvent* event) {
    switch (event->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseMove: {
            // Проверка того, что отправителем события является объект с типом QTableView
            auto table = dynamic_cast<QTableView *> (obj);
            if (!table) {
                table = dynamic_cast<QTableView *> (obj->parent());
                if (!table) {
                    break;
                }
            }

            // Проверка того, что положение мышки находится в ячейки таблицы
            auto mouseEvent = static_cast<QMouseEvent *> (event);
            auto pos = mouseEvent->pos();
            auto index = table->indexAt(pos);
            if (!index.isValid()) {
                break;
            }

            // Проверяем, что клик произошел на левой половине ячейки
            bool left = pos.x() < table->visualRect(index).center().x();
            auto side = left ? Side::Left : Side::Right;

            // Проверяем, что новые значения уникальные -- ненужные действия не нужны
            if (index == mIndex && side == selectedSide) {
                break;
            }

            mIndex = index;
            selectedSide = side;

            table->viewport()->update();

            break;
        }

        default:
            break;
    }

    return false;
}

void LineDays_CellDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();

    QRect rect = option.rect;

    QImage dayImage = index.model()->data(index, LineDaysTableModel::DayKind_Image_Day_Role).value<QImage>();
    QImage nightImage = index.model()->data(index, LineDaysTableModel::DayKind_Image_Night_Role).value<QImage>();

    // Отступ между иконками в ячейке
    auto indent = 2;
    auto size = qMin(rect.size().width(), rect.size().height()) - indent / 2;

    // TODO: исправить ошибку "QImage::scaled: Image is a null image"
    dayImage = dayImage.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    nightImage = nightImage.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    painter->drawImage(rect.x(), rect.y(), dayImage);
    painter->drawImage(rect.x() + size + indent, rect.y(), nightImage);

    painter->restore();

    QStyleOptionViewItem itemOption(option);
    initStyleOption(&itemOption, index);

    // Обработка при выделении ячейки делегата
    if (itemOption.state & QStyle::State_Selected && index == mIndex && selectedSide != Side::None) {
        QRect& rect = itemOption.rect;
        switch (selectedSide) {
            case Side::Left:
                rect.setWidth(rect.width() / 2);
                break;

            case Side::Right:
                rect.setX(rect.x() + rect.width() / 2);
                break;

            default:
                break;
        }

        QColor color = itemOption.palette.color(QPalette::Highlight);
        color.setAlpha(180);
        itemOption.palette.setColor(QPalette::Highlight, color);
    }

    QStyledItemDelegate::paint(painter, itemOption, index);
}
