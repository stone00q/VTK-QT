#include "vtkgeometrywidget.h"

vtkGeometryWidget::vtkGeometryWidget(QWidget *parent)
    : QVTKOpenGLNativeWidget(parent)
{
    this->geoReader=vtkSmartPointer<vtkOCCTReader>::New();

    this->renderer = vtkSmartPointer<vtkRenderer>::New();
    this->renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    this->renderWindow->AddRenderer(renderer);
    this->setRenderWindow(renderWindow);

    //获取交互器
    this->qvtkInteractor=this->interactor();
}
vtkGeometryWidget::~vtkGeometryWidget()
{

}
void vtkGeometryWidget::SetInputData(QString fileName)
{

    this->geoReader->SetFileName(fileName.toStdString().c_str());
    if(fileName.endsWith(".step")||fileName.endsWith(".stp"))
    {
        this->geoReader->SetFileFormat(vtkOCCTReader::Format::STEP);// 执行读取操作
    }else if(fileName.endsWith(".iges"))
    {
        this->geoReader->SetFileFormat(vtkOCCTReader::Format::IGES);
    }else
    {
        cout<<"报错"<<endl;
    }
    this->geoReader->Update();
    /*官方test代码写法：
    auto mapper = vtkSmartPointer<vtkCompositePolyDataMapper>::New();
    mapper->SetInputConnection(this->geoReader->GetOutputPort());*/
    vtkMultiBlockDataSet* multiBlock = vtkMultiBlockDataSet::SafeDownCast(this->geoReader->GetOutput());
    vtkPolyData* polyData = vtkPolyData::SafeDownCast(multiBlock->GetBlock(0));//假设第一块渲染
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polyData);
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    this->renderer->AddActor(actor);
    this->renderWindow->Render();
}
