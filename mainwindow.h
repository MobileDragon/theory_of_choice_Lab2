#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWindow>
#include <QMainWindow>
#include <QDebug>
#include <QLineEdit>
#include "mylineedit.h"
#include <QStandardItem>
#include <QTableView>
#include <QHBoxLayout>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void tabChanged(QStandardItem* item);
    void tablePresed(QModelIndex vodelIndex);
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_comboBox_2_currentIndexChanged(const QString &arg1);
    void lineChanged(MyLineEdit *lE);//изменение эначения компетентности эксперта



private:
    QPoint mousPos;
    Ui::MainWindow *ui;
    void build();
    void rebuild();
    bool isCorrect();//корректность заполнения всех данных-запускает старт
    bool isFull();
    QSize getSize();
    void calculate();//вычисляет веса цедей Z
    QMap<int, float> kompetent;//оценки компетентности эксмертов
    int kolTables;//число критериев
    int kolVariants;//число вариантов
    QTableView* tableRate;//таблица
    QStandardItemModel  *model;
    QHBoxLayout *hLayout;
    int colColumn;
    int colRow;
    int oldColRow;
    int winZ;//номер столбца победителя, если winZ = -1 такого нет
    bool valueChanged;
    //ответ
    int numVin;
    float *weightTarget;//массив результирующих весов целей
    void viewerTheWin();
    //
    void findVin();


    QVector< MyLineEdit* > linesKompetent;
    QVector< QLabel* > labelForKompetent;
    QVector< QVector<float> > tableContent;//содержимое таблици
};

#endif // MAINWINDOW_H
