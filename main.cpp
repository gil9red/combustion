#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>


// TODO: указатели 0 заменить на nullptr
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    try {
        MainWindow w;
        w.show();
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
