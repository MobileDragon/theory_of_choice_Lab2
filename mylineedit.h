#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H
#include <QLineEdit>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit MyLineEdit(int id);
    int myid;
    int getID();
signals:
  void MySignal(MyLineEdit *);

private:
    void build(int id);

private slots:
     void myChanged(QString s);

};

#endif // MYLINEEDIT_H
