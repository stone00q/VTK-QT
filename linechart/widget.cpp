#include "widget.h"
#include "ui_widget.h"
// #include "class_reader.h"

#include <vtkDataSetMapper.h>
#include <vtkSmartPointer.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkCylinderSource.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>

#include <qgridlayout.h>
#include <qpushbutton.h>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //创建自定义widget实例
    lineChartWidget = new vtkLineChartWidget(this);
    //在当前widget上布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(lineChartWidget);

    //数据读入
    lineChartWidget->SetInputData(R"(D:\Project\VTK_QT\data\test.csv)");

    //添加线,只能点一次
    QPushButton* addFirstPlot = new QPushButton("添加一条线", this);
    layout->addWidget(addFirstPlot);

    connect(addFirstPlot,&QPushButton::clicked,this,&Widget::addFirstPlot_clicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::addFirstPlot_clicked()
{
    this->lineChartWidget->AddPlot("Time","Sensor1",1);
}

