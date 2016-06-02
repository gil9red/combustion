#include "mainwindow.h"
#include <QApplication>

#include <parserpuzzlefile.h>

int main(int argc, char *argv[])
{
    ParserPuzzleFile::parse("../INFO/given_pussle.txt");

    return 0;

//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
}
