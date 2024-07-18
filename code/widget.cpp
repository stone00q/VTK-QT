#include "widget.h"
#include "ui_widget.h"
#include "class_reader.h"

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

    /*QPushButton* BackgroundButton = new QPushButton("Background: Blue", this);
    QPushButton* SolidColorButton = new QPushButton("Solid Color: Green", this);
    QPushButton* TButton = new QPushButton("Variable: T", this);
    QPushButton* XButton = new QPushButton("Variable: X", this);
    QPushButton* CutPlaneButton = new QPushButton("CutWidget", this);
    QPushButton* ColseCutWidget = new QPushButton("Close cutwidget", this);
    QPushButton* CutButton = new QPushButton("Cut", this);
    QPushButton* StreamSeedButton = new QPushButton("StreamSeed: point cloud", this);
    QPushButton* StreamTraceButton =new QPushButton("StreamTrace",this);
    layout->addWidget(BackgroundButton);
    layout->addWidget(SolidColorButton);
    layout->addWidget(TButton);
    layout->addWidget(XButton);
    layout->addWidget(CutPlaneButton);
    layout->addWidget(ColseCutWidget);
    layout->addWidget(CutButton);
    layout->addWidget(StreamSeedButton);
    layout->addWidget(StreamTraceButton);*/
    //需要测试的是颜色映射能在setsolidcolor和colormap之间切换，以及产生Slice1对slice1的颜色映射
    QPushButton* CutPlaneButton = new QPushButton("CutWidget", this);
    QPushButton* CutButton = new QPushButton("Cut", this);
    QPushButton* basicButton1 = new QPushButton("基础basicActor设置固体颜色绿色", this);
    QPushButton* basicButton2 = new QPushButton("基础basicActor设置颜色映射", this);
    QPushButton* slice1Button1 = new QPushButton("Slice1基础颜色", this);
    QPushButton* slice1Button2 = new QPushButton("Slice1颜色映射", this);
    layout->addWidget(CutPlaneButton);
    layout->addWidget(CutButton);
    layout->addWidget(basicButton1);
    layout->addWidget(basicButton2);
    layout->addWidget(slice1Button1);
    layout->addWidget(slice1Button2);
    //数据的读入
    TecplotReader reader(R"(D:\Project\VTK_QT\data\Tur_Merge_Field_[2000].dat)");
    vtkMultiBlockDataSet* multiBlock=reader.readTecplot();

    //设置vtktecplotwidget
    tecplotWidget->SetInputData(multiBlock);

    // 连接信号槽
    connect(CutPlaneButton,&QPushButton::clicked,this,&Widget::CutPlaneButton_clicked);
    connect(CutButton,&QPushButton::clicked,this,&Widget::CutButton_clicked);
    connect(basicButton1,&QPushButton::clicked,this,&Widget::basicButton1_clicked);
    connect(basicButton2,&QPushButton::clicked,this,&Widget::basicButton2_clicked);
    connect(slice1Button1,&QPushButton::clicked,this,&Widget::slice1Button1_clicked);
    connect(slice1Button2,&QPushButton::clicked,this,&Widget::slice1Button2_clicked);
    /*connect(BackgroundButton, &QPushButton::clicked, this, &Widget::BackgroundButton_clicked);
    connect(SolidColorButton, &QPushButton::clicked, this, &Widget::SolidColorButton_clicked);
    connect(TButton, &QPushButton::clicked, this, &Widget::TButton_clicked);
    connect(XButton, &QPushButton::clicked, this, &Widget::XButton_clicked);
    connect(CutPlaneButton,&QPushButton::clicked,this,&Widget::CutPlaneButton_clicked);
    connect(ColseCutWidget,&QPushButton::clicked,this,&Widget::ColseCutWidget_clicked);
    connect(CutButton,&QPushButton::clicked,this,&Widget::CutButton_clicked);
    connect(StreamSeedButton,&QPushButton::clicked,this,&Widget::StreamSeedButton_clicked);
    connect(StreamTraceButton,&QPushButton::clicked,this,&Widget::StreamTraceButton_clicked);*/
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
    //tecplotWidget->SetColorMapVariable("T");
    tecplotWidget->SetColorMappingFlag(true);
}
void Widget::XButton_clicked()
{
    //x坐标颜色映射
    //tecplotWidget->SetColorMapVariable("p");
    tecplotWidget->SetColorMappingFlag(true);
}
void Widget::CutPlaneButton_clicked()
{
    tecplotWidget->SetCutPlaneWidget();
}
void Widget:: ColseCutWidget_clicked()
{
    tecplotWidget->SetCutPlaneWidget(false);
}
void Widget::CutButton_clicked()
{
    QString tmpName = tecplotWidget->AddCutPlane();
    std::cout << endl <<"cutplane name:" << tmpName.toStdString();
}
void Widget::StreamSeedButton_clicked()
{
    tecplotWidget->SetStreamTrancerSeed();
}
void Widget::StreamTraceButton_clicked()
{
    tecplotWidget->SetStreamTrancerApply();
}


void Widget::basicButton1_clicked()
{
    tecplotWidget->SetColorMapObject("basicActor");
    tecplotWidget->SetSolidColor(QColorConstants::Green);
}
void Widget::basicButton2_clicked()
{
    tecplotWidget->SetColorMapObject("basicActor");
    tecplotWidget->SetColorMapVariable("p");
}
void Widget::slice1Button1_clicked()
{
    tecplotWidget->SetColorMapObject("Slice1");
    tecplotWidget->SetSolidColor(QColorConstants::Red);
}
void Widget::slice1Button2_clicked()
{
    tecplotWidget->SetColorMapObject("Slice1");
    tecplotWidget->SetColorMapVariable("X");
}
