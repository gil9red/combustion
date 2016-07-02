#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    try {
        MainWindow w;
        w.show();
        w.load(":/default_puzzle");

        return a.exec();

    } catch (std::exception& e) {
        QString text = QString("ERROR: %1.").arg(e.what());
        QMessageBox::critical(NULL, QString(), text);
        qCritical() << text;
        a.quit();
        return 1;
    }
}
