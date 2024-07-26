#include "widget.h"
#include "ui_widget.h"

#include <qgridlayout.h>
#include <qpushbutton.h>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //创建自定义widget实例
    geometryWidget = new vtkGeometryWidget(this);
    //在当前widget上布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(geometryWidget);

    QPushButton* StepButton = new QPushButton("读取step文件",this);
    layout->addWidget(StepButton);
    QPushButton* IgesButton = new QPushButton("读取iges文件",this);
    layout->addWidget(IgesButton);
    connect(StepButton,&QPushButton::clicked,this,&Widget::StepButton_clicked);
    connect(IgesButton,&QPushButton::clicked,this,&Widget::IgesButton_clicked);
}

Widget::~Widget()
{
    delete ui;
}
void Widget::StepButton_clicked()
{//输入文件所在路径
    QString name = "D:/Project/VTK_QT/data/cylinder.stp";
    this->geometryWidget->SetInputData(name);
}
void Widget::IgesButton_clicked()
{   QString name = "D:/Project/VTK_QT/data/dlr-f4.iges";
    this->geometryWidget->SetInputData(name);
}
