#include "vtkgeometrywidget.h"

vtkGeometryWidget::vtkGeometryWidget(QWidget *parent)
    : QVTKOpenGLNativeWidget(parent)
{
    this->geoReader=vtkSmartPointer<vtkOCCTReader>::New();
    this->lut = vtkSmartPointer<vtkLookupTable>::New();
    this->renderer = vtkSmartPointer<vtkRenderer>::New();
    this->renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    this->renderWindow->AddRenderer(renderer);
    this->setRenderWindow(renderWindow);

    this->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    this->actor = vtkSmartPointer<vtkActor>::New();
    this->actor->SetMapper(mapper);
    //获取交互器
    this->qvtkInteractor=this->interactor();
    QColor lightBlue(82, 87, 110);
    renderer->SetBackground(lightBlue.redF(), lightBlue.greenF(), lightBlue.blueF());
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
    /*官方test代码写法：在颜色映射会报错
    this->mapper = vtkSmartPointer<vtkCompositePolyDataMapper>::New();
    this->mapper->SetInputConnection(this->geoReader->GetOutputPort());*/
    vtkMultiBlockDataSet* multiBlock = vtkMultiBlockDataSet::SafeDownCast(this->geoReader->GetOutput());
    vtkPolyData* polyData = vtkPolyData::SafeDownCast(multiBlock->GetBlock(0));//取出第一块
    this->mapper->SetInputData(polyData);
    this->mapper->SetScalarVisibility(false);

    this->renderer->AddActor(this->actor);
    this->renderer->ResetCamera();
    this->renderWindow->Render();
}
void vtkGeometryWidget::SetColorMapVisibility(bool flag, QString propName)
{
    int propId =this->mapper->GetInput()->GetCellData()->SetActiveScalars(propName.toStdString().c_str()); // 设置标量
    vtkDataArray* propDate = this->mapper->GetInput()->GetCellData()->GetArray(propId);
    this->mapper->SetScalarVisibility(true);  //标量可见
    this->mapper->SetScalarModeToUseCellData();
    lut->SetNumberOfColors(256);
    this->lut->Build();
    // 设置查找表的颜色范围
    this->mapper->SetScalarRange(propDate->GetRange());
    this->mapper->SetLookupTable(lut);
    this->renderer->ResetCamera();
    this->renderWindow->Render();
}
