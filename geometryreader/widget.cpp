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
    QPushButton* ColorButton = new QPushButton("颜色映射",this);
    layout->addWidget(ColorButton);
    connect(StepButton,&QPushButton::clicked,this,&Widget::StepButton_clicked);
    connect(IgesButton,&QPushButton::clicked,this,&Widget::IgesButton_clicked);
    connect(ColorButton,&QPushButton::clicked,this,&Widget::ColorButton_clicked);
}

Widget::~Widget()
{
    delete ui;
}
void Widget::StepButton_clicked()
{//输入文件所在路径
    QString name = "D:/Project/VTK_QT/data/f6_clean_dpwII_v2_whole_cf2 v3.step";
    this->geometryWidget->SetInputData(name);
}
void Widget::IgesButton_clicked()
{   QString name = "D:/Project/VTK_QT/data/Cylinder.iges";
    this->geometryWidget->SetInputData(name);
}
void Widget::ColorButton_clicked()
{
    this->geometryWidget->SetColorMapVisibility();
}
