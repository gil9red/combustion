#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>


// TODO: проверка исключений в наследнике QApplication
/*
#include <QtGui>
#include <QApplication>
class MyApplication : public QApplication {
public:
  MyApplication(int& argc, char ** argv) :
    QApplication(argc, argv) { }
  virtual ~MyApplication() { }

  // reimplemented from QApplication so we can throw exceptions in slots
  virtual bool notify(QObject * receiver, QEvent * event) {
    try {
      return QApplication::notify(receiver, event);
    } catch(std::exception& e) {
      qCritical() << "Exception thrown:" << e.what();
    }
    return false;
  }

int main(int argc, char *argv[]) {
  MyApplication app(argc, argv);
  // ...
}
*/


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    try {
        MainWindow w;
        w.show();
        w.load(":/default_puzzle");

        return a.exec();

    } catch (const std::exception& e) {
        QString text = QString("ERROR: %1.").arg(e.what());
        QMessageBox::critical(NULL, QString(), text);
        qCritical() << text;
        a.quit();
        return 1;
    }
}
