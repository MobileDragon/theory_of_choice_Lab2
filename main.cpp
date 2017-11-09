#include "mainwindow.h"
#include <QApplication>
//программа для нахождения найлучшего варианта(чего либо) на основании оценок данных экспертами,
//и показателя достоверности каждого эксперта
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //w.setFixedSize(251,180);
    //w.setFixedWidth(251);
    //w.setMinimumHeight(204);


    return a.exec();
}
