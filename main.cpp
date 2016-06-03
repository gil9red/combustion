#include "mainwindow.h"
#include <QApplication>

#include <parserpuzzlefile.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    try {
        QList<Busman*> busmanList = ParserPuzzleFile::parse("../combustion/INFO/given_pussle.txt");
        qDebug() << busmanList.size();
        foreach (Busman *busman, busmanList) {
            qDebug() << busman->busNum << busman->selectLines;
        }

    } catch(std::exception& e) {
        qCritical() << e.what();
    }

    return 0;

//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
}
