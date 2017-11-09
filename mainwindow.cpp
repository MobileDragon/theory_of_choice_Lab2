#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    build();
}

MainWindow::~MainWindow()
{
    delete[]weightTarget;
    delete ui;
}

void MainWindow::build()
{
    model=0;
    weightTarget=0;
    numVin=-1;
    valueChanged=false;
    hLayout = new QHBoxLayout();
    colColumn=2;
    oldColRow=0;
    colRow=2;
    tableRate=new QTableView();
    tableRate->setFont(QFont("Arial",10,5000));
    tableRate->verticalHeader()->hide();
    tableRate->horizontalHeader()->hide();
    ui->verticalLayout_4->addWidget(tableRate);
    ui->comboBox->setFixedWidth(50);
    ui->comboBox_2->setFixedWidth(50);
    for(int i=2; i<11; ++i)
    {
        ui->comboBox->addItem(QString().setNum(i,10) );
        ui->comboBox_2->addItem(QString().setNum(i,10) );
    }

}

void MainWindow::rebuild()
{
    if(oldColRow!=colRow)
    {
        foreach (MyLineEdit* line, linesKompetent)
        {
            line->deleteLater();
        }
        foreach (QLabel* line, labelForKompetent)
        {
            line->deleteLater();
        }
        linesKompetent.clear();
        labelForKompetent.clear();
        kompetent.clear();
        hLayout->deleteLater();

        for(int i=0; i<colRow; i++)
        {
            hLayout = new QHBoxLayout();
            QLabel *label = new QLabel(QString("Компетентность Э%1:").arg(i+1));

            MyLineEdit *line = new MyLineEdit(i);
            line->setMinimumWidth(60);
            //
            linesKompetent.append(line);
            labelForKompetent.append(label);
            connect(line, SIGNAL(MySignal(MyLineEdit*)) ,this,SLOT(lineChanged(MyLineEdit*) ));
            //
            hLayout->addWidget(label);
            hLayout->addWidget(line);
            ui->verticalLayout_8->addLayout(hLayout);

        }
    }

    //
    QStandardItemModel *t_model=model;
    model = new QStandardItemModel;
    QStandardItem *item;
    //

    item = new QStandardItem("Э/Z");
    item->setEditable(false);
    item->setCheckable(false);
    item->setEnabled(false);

    item->setBackground(QBrush(QColor(250,230,170)) );
    item->setFont(QFont("Arial",10,6500));
    model->setItem(0, 0, item);//
    //


    for (int i=0; i<colRow; i++)
    {
        QString s = QString("Э%1").arg(i+1);
        item = new QStandardItem(s);
        item->setFont(QFont("Arial",10,6500));
        item->setEditable(false);
        item->setSelectable(false);
        item->setBackground(QBrush(QColor(250,230,170)) );
        //
        model->setItem(i+1, 0, item);
    }
    for(int i=0; i<colColumn; i++)
    {
        QString s = QString("Z%1").arg(i+1);
        item = new QStandardItem(s);
        item->setFont(QFont("Arial",10,6500));
        item->setEditable(false);
        item->setSelectable(false);
        item->setBackground(QBrush(QColor(250,230,170)) );
        //
        model->setItem(0, i+1, item);
    }



    //определение количества ячеек которве можно переписать из предыдущей таблицы//ok
    int t_row=oldColRow;//во время увеличения числа строк в таблице может быть строка с результирующими весами
    oldColRow=colRow;
    int t_column=0;
    if(t_model!=0)
    {
        t_column = t_model->columnCount()-1;//-1 так как имена заголовков не считаются
        //t_row= t_model->rowCount()-1;
    }
    for (int i=0; i<colRow; i++)
    {
        for(int j=0;j<colColumn; j++)
        {
            QString s = "";
            if(t_row > i && t_column > j)
                s = t_model->takeItem(i+1,j+1)->text();

            item = new QStandardItem(s);

            item->setFont(QFont("Arial",10,6500));
            model->setItem(i+1, j+1, item);
        }
        //

    }
    //
    if(t_model!=0)
        t_model->deleteLater();
    if(numVin >= 0)
    {
        QStandardItem *item2;
        for(int j=0;j<=colColumn; j++)
        {
            item2 = new QStandardItem("u1");
            item2->setFont(QFont("Arial",18,100));//указать веса
            model->setItem(colRow+1, j, item2);
        }
    }


    tableRate->setModel(model);

    connect(model, SIGNAL(itemChanged(QStandardItem*)) ,this,SLOT(tabChanged(QStandardItem*) ));//
    connect(tableRate, SIGNAL(pressed(QModelIndex)) ,this,SLOT(tablePresed(QModelIndex) ));

    for(int i=0; i <= colColumn; i++)//
    {
        tableRate->horizontalHeader()->setSectionResizeMode(i,QHeaderView::ResizeToContents);
    }

    tableRate->setFixedSize(getSize());



}

QSize MainWindow::getSize()
{
    QSize tableSize;
    int i=0;
    int height=0;
    int currentHeight=tableRate->rowHeight(i);
    while(currentHeight > 0)
    {
        height+=currentHeight;
        ++i;
        currentHeight=tableRate->rowHeight(i);
    }
    height+=i/2+1;
    i=0;
    int width=0;
    int currentWidth=tableRate->columnWidth(i);
    while(currentWidth > 0)
    {
        width+=currentWidth;
        ++i;
        currentWidth=tableRate->columnWidth(i);
    }
    width+=i/2+1;
    //width = tableCreterion->columnWidth(0)+20;
    tableSize.setHeight(height);
    tableSize.setWidth(width);
    return tableSize;
}

void MainWindow::calculate()
{
    float sum=0;//сумма оценок компетентности экспертов
    for(int i=0; i<colRow; i++)
    {
        sum += kompetent[i];
    }
    //вычисление относительных оценок компетентности
    float *expertWeight = new float[colRow];
    for(int i=0; i<colRow; i++)
    {
        expertWeight[i]=kompetent[i]/sum;
    }
    //подсчет весов целей
    delete[]weightTarget;
    weightTarget = new float[colRow];
    numVin=0;
    float max=0;
    for(int i=0; i<colColumn; i++)
    {
        for(int j=0; j<colRow; j++)
        {
            weightTarget[i] = weightTarget[i] + tableContent[j][i]*expertWeight[j];
        }
        if(max<weightTarget[i])
        {
            max=weightTarget[i];
            numVin=i;
        }
        //
    }



    delete[]expertWeight;
}

bool MainWindow::isCorrect()
{
    if(kompetent.size()!=colRow)
        return false;
    if(!isFull())
        return false;
    return true;
}

void MainWindow::viewerTheWin()
{
    if(isCorrect())
    {
        for(int i=1; i<=colRow; i++)
        {
            model->item(i,numVin)->setBackground(QBrush(QColor(80,250,50) ));
        }
    }
}

void MainWindow::findVin()
{

    if(isFull())
    {
        qDebug() << "УБРАТЬ 1";
        calculate();
        qDebug() << "УБРАТЬ 2";
    }
    else
        numVin = -1;
    rebuild();

}



void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    int kol=arg1.toInt();
    colRow=kol;

    //
    rebuild();
    findVin();
    this->adjustSize();//передел размера окна
}

void MainWindow::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    int kol=arg1.toInt();
    colColumn=kol;

    //
    rebuild();
    findVin();
    this->adjustSize();
}


void MainWindow::lineChanged(MyLineEdit * lE)
{


    if(valueChanged)
        return;
    int id = lE->getID();
    bool ok;
    float f=lE->text().toFloat(&ok);
    if(lE->text().length() > 0 && ok)
    {
        kompetent[id]=f;
    }
    else
    {
        kompetent.remove(id);
        valueChanged=true;
        lE->setText("");
        valueChanged=false;
    }
    findVin();
}

void MainWindow::tablePresed(QModelIndex vodelIndex)//ИЗМЕНЕНИЕ РАЗМЕРА
{
    tableRate->setFixedSize(getSize());
    this->adjustSize();
}

void MainWindow::tabChanged(QStandardItem* item)
{
    if(!valueChanged)
    {
        bool ok;
        item->text().toFloat(&ok);

        if(!(ok))
        {
            valueChanged=true;
            item->setText("");
            valueChanged=false;
        }
        findVin();
    }
}

bool MainWindow::isFull()
{
    if(model == 0)
            return false;
    if(kompetent.size() != colRow)
        return false;
    tableContent.clear();
    for(int i=1; i<= colRow; ++i)
    {
        QVector<float> rowValues;
        for(int j=1; j<= colColumn; j++)
        {
            QString text = model->item(i,j)->text();
            if(text.length() == 0)
                return false;
            float val = text.toFloat();
            if(val < 0)
            {
                tableContent.clear();
                return false;
            }
            rowValues.append( val );
        }
        tableContent.append(rowValues);
    }
    return true;
}
