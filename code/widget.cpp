#include "widget.h"
#include "ui_widget.h"
// #include "class_reader.h"
#include "reader2.h"

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
/****
    TecplotReader reader(R"(D:\Project\VTK_QT\data\Tur_Merge_Field_[2000].dat)");
    vtkSmartPointer<vtkMultiBlockDataSet> multiBlock=reader.readTecplot();
    vtkSmartPointer<vtkUnstructuredGrid> ug = vtkUnstructuredGrid::SafeDownCast(multiBlock->GetBlock(0));

    auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(ug);
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    auto renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    ui->qvtkWidget->setRenderWindow(renderWindow);

    colormap.setInputData(ug);

    connect(ui->rhoButton,&QPushButton::clicked,this,&Widget::on_rhoButton_clicked);
    connect(ui->pButton,&QPushButton::clicked,this,&Widget::on_pButton_clicked);****/


    //创建自定义widget实例
    tecplotWidget = new vtkTecplotWidget(this);
    //在当前widget上布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(tecplotWidget);

    QPushButton* BackgroundButton = new QPushButton("Background: Blue", this);
    QPushButton* SolidColorButton = new QPushButton("Solid Color: Green", this);
    QPushButton* TButton = new QPushButton("Variable: T", this);
    QPushButton* XButton = new QPushButton("Variable: X", this);
    QPushButton* CutPlaneButton = new QPushButton("CutPlane", this);
    QPushButton* CutButton = new QPushButton("Cut", this);
    QPushButton* StreamSeedButton = new QPushButton("StreamSeed: point cloud", this);
    QPushButton* StreamTraceButton =new QPushButton("StreamTrace",this);
    layout->addWidget(BackgroundButton);
    layout->addWidget(SolidColorButton);
    layout->addWidget(TButton);
    layout->addWidget(XButton);
    layout->addWidget(CutPlaneButton);
    layout->addWidget(CutButton);
    layout->addWidget(StreamSeedButton);
    layout->addWidget(StreamTraceButton);
    //数据的读入
    TecplotReader reader(R"(D:\Project\VTK_QT\data\Tur_Merge_Field_[2000].dat)");
    vtkMultiBlockDataSet* multiBlock=reader.readTecplot();

    //设置vtktecplotwidget
    tecplotWidget->SetInputData(multiBlock);

    // 连接信号槽
    connect(BackgroundButton, &QPushButton::clicked, this, &Widget::BackgroundButton_clicked);
    connect(SolidColorButton, &QPushButton::clicked, this, &Widget::SolidColorButton_clicked);
    connect(TButton, &QPushButton::clicked, this, &Widget::TButton_clicked);
    connect(XButton, &QPushButton::clicked, this, &Widget::XButton_clicked);
    connect(CutPlaneButton,&QPushButton::clicked,this,&Widget::CutPlaneButton_clicked);
    connect(CutButton,&QPushButton::clicked,this,&Widget::CutButton_clicked);
    connect(StreamSeedButton,&QPushButton::clicked,this,&Widget::StreamSeedButton_clicked);
    connect(StreamTraceButton,&QPushButton::clicked,this,&Widget::StreamTraceButton_clicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::BackgroundButton_clicked()
{
    //渲染窗口为浅蓝色
    QColor color=QColor(173, 216, 230);
    tecplotWidget->SetBackgroundColor(color);
}
void Widget::SolidColorButton_clicked()
{
    //固体为绿色
    tecplotWidget->SetSolidColor(QColorConstants::Green);
}
void Widget::TButton_clicked()
{
    //t颜色映射
    tecplotWidget->SetColorMapVariable("T");
    tecplotWidget->SetColorMappingFlag(true);
}
void Widget::XButton_clicked()
{
    //x坐标颜色映射
    tecplotWidget->SetColorMapVariable("X");
    tecplotWidget->SetColorMappingFlag(true);
}
void Widget::CutPlaneButton_clicked()
{
    tecplotWidget->SetCutPlane();
}
void Widget::CutButton_clicked()
{
    tecplotWidget->SetCutPlaneFlag(true);
}
void Widget::StreamSeedButton_clicked()
{
    tecplotWidget->SetStreamTrancerSeed();
}
void Widget::StreamTraceButton_clicked()
{
    tecplotWidget->SetStreamTrancerApply();
}
