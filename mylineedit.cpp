#include "mylineedit.h"

MyLineEdit::MyLineEdit(int id):QLineEdit()
{
    build(id);
}

int MyLineEdit::getID()
{
    return myid;
}

void MyLineEdit::build(int id)
{
    myid=id;
    connect(this, SIGNAL(textChanged(QString)) ,this,SLOT(myChanged(QString) ));

}
void MyLineEdit::myChanged(QString s)
{
    emit MySignal(this);
}
