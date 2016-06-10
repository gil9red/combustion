#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>


// TODO: указатели 0 заменить на nullptr
// TODO: исправить падение при закрытии окна
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // TODO: помнить размеры, положение и состояние виджетов
    try {
        MainWindow w;
        w.resize(970, 550);
        w.show();

        // FIXME: linedaystable исправить баг при повторной загрузке
        w.load("../combustion/INFO/given_pussle.txt");

        return a.exec();

    } catch (std::exception& e) {
        QString text = QString("ERROR: %1.").arg(e.what());
        QMessageBox::critical(NULL, QString(), text);
        qCritical() << text;
        a.quit();
        return 1;
    }
}
