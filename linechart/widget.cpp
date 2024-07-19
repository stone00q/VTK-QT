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

    //数据的读入
    TecplotReader reader(R"(D:\Project\VTK_QT\data\Tur_Merge_Field_[2000].dat)");
    vtkMultiBlockDataSet* multiBlock=reader.readTecplot();
    //设置vtktecplotwidget
    lineChartWidget->SetInputData(multiBlock);

    // 连接信号槽
}

Widget::~Widget()
{
    delete ui;
}

