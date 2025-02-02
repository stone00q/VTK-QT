#include "vtkTecplotWidget.h"

vtkTecplotWidget::vtkTecplotWidget(QWidget *parent)
    : QVTKOpenGLNativeWidget(parent)
{
    // 初始化渲染器和渲染窗口等所有功能共享部分
    this->multiBlock=vtkSmartPointer<vtkMultiBlockDataSet>::New();
    this->unstructuredGrid=vtkSmartPointer<vtkUnstructuredGrid>::New();
    this->pointData=vtkSmartPointer<vtkPointData>::New();
    //this->scalars=vtkSmartPointer<vtkDataArray>::New();
    //scalars=nullptr;
    this->varNum = 0;

    this->renderer = vtkSmartPointer<vtkRenderer>::New();
    this->renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    this->setRenderWindow(renderWindow);

    //获取交互器
    this->qvtkInteractor=this->interactor();
    this->cutPlaneWidget = vtkSmartPointer<vtkImplicitPlaneWidget2>::New();
    this->cutPlaneRep=vtkSmartPointer<vtkImplicitPlaneRepresentation>::New();

    this->seedSphereWidget=vtkSmartPointer<vtkSphereWidget>::New();
    seedPointNum=100;
}

vtkTecplotWidget::~vtkTecplotWidget() {

}

void vtkTecplotWidget::SetInputData(vtkMultiBlockDataSet* inputData)
{
    if (inputData == nullptr) {
        std::cerr << "Input data is nullptr." << std::endl;
        return;
    }
    this->multiBlock = inputData;
    this->unstructuredGrid=vtkUnstructuredGrid::SafeDownCast(this->multiBlock->GetBlock(0));

    //添加一个速度向量
    //auto calculator=vtkSmartPointer<vtkArrayCalculator>::New();
    //calculator->SetInputData(this->unstructuredGrid);
    //calculator->AddScalarArrayName("u");
    //calculator->AddScalarArrayName("v");
    //calculator->AddScalarArrayName("w");
    //calculator->SetResultArrayName("velocity");
    //calculator->SetFunction("u*iHat + v*jHat + w*kHat");
    //calculator->Update();

    //this->unstructuredGrid=calculator->GetUnstructuredGridOutput();
    this->pointData = this->unstructuredGrid->GetPointData();
    varNum = pointData->GetNumberOfArrays();
    //开始basic渲染
    auto basicMapper=vtkSmartPointer<vtkDataSetMapper>::New();
    auto basicActor=vtkSmartPointer<vtkActor>::New();
    this->actorsMap["basicActor"]=basicActor;
    this->mappersMap["basicMapper"]=basicMapper;
    basicMapper->SetInputData(unstructuredGrid);
    basicActor->SetMapper(basicMapper);
    this->renderer->AddActor(basicActor);
    this->renderWindow->Render();

}

void vtkTecplotWidget::SetBackgroundColor(QColor color)
{
    //？？默认颜色0-255
    int r=color.red();
    int g=color.green();
    int b=color.blue();

    double normalizedR = r / 255.0;
    double normalizedG = g / 255.0;
    double normalizedB = b / 255.0;

    this->renderer->SetBackground(normalizedR, normalizedG, normalizedB);
    //？？是否每次都要设置后都要重新渲染
    this->renderWindow->Render();
}

QColor vtkTecplotWidget::GetBackgroundColor()
{
    double* rgb = renderer->GetBackground();
    int r = static_cast<int>(rgb[0] * 255);
    int g = static_cast<int>(rgb[1] * 255);
    int b = static_cast<int>(rgb[2] * 255);
    return QColor(r, g, b);
}

void vtkTecplotWidget::SetSolidColor(QColor  RGBA)
{
    //颜色转换
    int r=RGBA.red();
    int g=RGBA.green();
    int b=RGBA.blue();
    double normalizedR = r / 255.0;
    double normalizedG = g / 255.0;
    double normalizedB = b / 255.0;

    auto actor=actorsMap["basicActor"];
    vtkProperty* property=actor->GetProperty();
    property->SetColor(normalizedR,normalizedG,normalizedB);
    this->renderWindow->Render();
}

QColor vtkTecplotWidget::GetSolidColor()
{
    auto actor=actorsMap["basicActor"];
    vtkProperty* property=actor->GetProperty();
    double* rgb=property->GetColor();
    int r = static_cast<int>(rgb[0] * 255);
    int g = static_cast<int>(rgb[1] * 255);
    int b = static_cast<int>(rgb[2] * 255);
    return QColor(r, g, b);
}

void vtkTecplotWidget::SetColorMapVariable(std::string selectedVar)
{
    /*
    int selectedId=-1;
    for (int i = 0; i < varNum; i++)
    {
        if (this->pointData->GetArrayName(i) == selectedVar) {
            selectedId = i;
            break;
        }
    }
    if(selectedId==-1)
    {
        std::cerr << "Illegal variable name" << std::endl;
    }
    //将选中变量设置为标量
    this->scalars=this->pointData->GetArray(selectedId);
    this->unstructuredGrid->GetPointData()->SetScalars(scalars);
    */
    int selectedID = this->unstructuredGrid->GetPointData()->SetActiveScalars(selectedVar.c_str());
     this->scalars=this->pointData->GetArray(selectedID);

    //设置颜色映射相关
    if (lutsMap.find("basicLut") == lutsMap.end())
    {
        auto basicLut=vtkSmartPointer<vtkLookupTable>::New();
        basicLut->SetTableRange(this->scalars->GetRange());  //设置颜色映射范围
        basicLut->SetNumberOfColors(256);
        basicLut->Build();
        lutsMap["basicLut"] = basicLut;
        auto basicScalarBarActor = vtkSmartPointer<vtkScalarBarActor>::New();
        basicScalarBarActor->SetLookupTable(basicLut);
        barActorsMap["basicScalarBarActor"]=basicScalarBarActor;

        auto mapper=mappersMap["basicMapper"];
        mapper->SetScalarRange(this->scalars->GetRange());
        mapper->SetLookupTable(basicLut);
        this->renderer->AddActor(basicScalarBarActor);

    }else
    {
        auto basicLut=lutsMap["basicLut"];
        basicLut->SetTableRange(this->scalars->GetRange());  //设置颜色映射范围
        basicLut->SetNumberOfColors(256);
        basicLut->Build();
        auto basicScalarBarActor=barActorsMap["basicScalarBarActor"];
        basicScalarBarActor->SetLookupTable(basicLut);

        auto mapper=mappersMap["basicMapper"];
        mapper->SetScalarRange(this->scalars->GetRange());
        mapper->SetLookupTable(basicLut);
        this->renderer->AddActor(basicScalarBarActor);
    }
}

void vtkTecplotWidget::SetColorMappingFlag(bool flag)//对于颜色映射paraview一remove就是整个actor remove
{
    //falg为true就执行颜色映射
    if(!flag && barActorsMap.find("basicScalarBarActor") != barActorsMap.end())
    {
        auto basicMapper = mappersMap["basicMapper"];
        basicMapper->ScalarVisibilityOff();  // 关闭颜色映射
        basicMapper->SetLookupTable(nullptr);  // 清除Lookup Table

        // 从renderer中移除ScalarBarActor
        if(barActorsMap.find("basicScalarBarActor") != barActorsMap.end())
        {
            this->renderer->RemoveActor(barActorsMap["basicScalarBarActor"]);
        }

        // 清空或恢复lutsMap与barActorsMap中的对象
        this->lutsMap.clear();
        this->barActorsMap.clear();
    }else if(flag)
    {
        this->renderWindow->Render();
    }
}

void vtkTecplotWidget::SetCutPlane()
{
    this->cutPlaneRep->SetPlaceFactor(1.0);
    this->cutPlaneRep->PlaceWidget(this->unstructuredGrid->GetBounds());
    //this->cutPlaneRep->SetHandleSize(100);
    this->cutPlaneRep->SetHandleColor(1.0,0.0,0.0);
    this->cutPlaneRep->SetEdgeColor(1.0,0.0,0.0);
    this->cutPlaneWidget->SetInteractor(this->qvtkInteractor);
    this->cutPlaneWidget->SetRepresentation(this->cutPlaneRep);
    //this->cutPlaneWidget->SetRepresentationToSurface();
    //this->cutPlaneWidget->SetResolution(1000);
    //this->cutPlaneWidget->SetRepresentationToOutline();

    //激活widget
    this->cutPlaneWidget->On();
    //更新渲染
    this->renderWindow->Render();
}

void vtkTecplotWidget::SetCutPlaneFlag(bool flag)
{
    //第一次初始化
    vtkSmartPointer<vtkPlane> cutPlane;
    vtkSmartPointer<vtkCutter> cutter;
    vtkSmartPointer<vtkPolyDataMapper> cutMapper;
    vtkSmartPointer<vtkActor> cutActor;
    if(flag&&actorsMap.find("cutActor")==actorsMap.end())
    {
        cutPlane=vtkSmartPointer<vtkPlane>::New();
        this->cutPlaneRep->GetPlane(cutPlane);
        cutter=vtkSmartPointer<vtkCutter>::New();
        cutter->SetCutFunction(cutPlane);
        cutter->SetInputData(this->unstructuredGrid);
        cutter->Update();
        cutMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
        cutMapper->SetInputConnection(cutter->GetOutputPort());

        cutActor=vtkSmartPointer<vtkActor>::New();
        this->actorsMap["cutActor"]=cutActor;
        cutActor->SetMapper(cutMapper);
        this->renderer->AddActor(cutActor);

        auto basicActor=actorsMap["basicActor"];
        basicActor->SetVisibility(0);
        this->renderer->Render();
    }
}

void vtkTecplotWidget::SetStreamTrancerSeed(int pointNum)
{
    this->seedPointNum=pointNum;
    auto colors=vtkSmartPointer<vtkNamedColors>::New();
    this->seedSphereWidget->PlaceWidget(this->unstructuredGrid->GetBounds());
    this->seedSphereWidget->SetPlaceFactor(0.5);
    this->seedSphereWidget->SetRepresentationToWireframe();
    this->seedSphereWidget->GetSphereProperty()->SetColor(colors->GetColor3d("BurlyWood").GetData());
    this->seedSphereWidget->GetSphereProperty()->SetOpacity(0.5);  // 设置透明度
    this->seedSphereWidget->SetInteractor(this->qvtkInteractor);
    this->seedSphereWidget->On();
}

void vtkTecplotWidget::SetStreamTrancerApply()
{
    double center[3];
    this->seedSphereWidget->GetCenter(center);
    double radius=this->seedSphereWidget->GetRadius();

    //？？下面这些代码都需要判断之前是否new过，即该功能是否打开过且未关闭
    //在球体内部生成指定数量的点
    auto pointSource=vtkSmartPointer<vtkPointSource>::New();
    pointSource->SetCenter(center);
    pointSource->SetRadius(radius);
    pointSource->SetNumberOfPoints(this->seedPointNum);
    pointSource->Update();

    this->pointData->SetActiveVectors("velocity");
    auto streamTracer=vtkSmartPointer<vtkStreamTracer>::New();
    streamTracer->SetInputData(this->unstructuredGrid);
    streamTracer->SetSourceConnection(pointSource->GetOutputPort());
    streamTracer->SetMaximumPropagation(100);
    streamTracer->SetIntegrationDirectionToBoth();
    streamTracer->SetIntegrator(vtkSmartPointer<vtkRungeKutta4>::New());

    // 生成流线
    vtkNew<vtkPolyDataMapper> streamMapper;
    streamMapper->SetInputConnection(streamTracer->GetOutputPort());

    vtkNew<vtkActor> streamActor;
    streamActor->SetMapper(streamMapper);
    //streamActor->GetProperty()->SetColor(1.0, 1.0, 0.0);  // 流线颜色

    auto basicActor=actorsMap["basicActor"];
    basicActor->SetVisibility(0);

    this->renderer->AddActor(streamActor);
    this->renderer->Render();

}
