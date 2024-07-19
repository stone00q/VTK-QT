#include "vtklinechartwidget.h"

vtkLineChartWidget::vtkLineChartWidget(QWidget *parent)
    :QVTKOpenGLNativeWidget(parent)
{
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
    this->view = vtkSmartPointer<vtkContextView>::New();
    this->renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    this->view->SetRenderWindow(this->renderWindow);
    this->setRenderWindow(this->renderWindow);
    //获取交互器
    this->qvtkInteractor = this->interactor();
    this->table = vtkSmartPointer<vtkTable>::New();
    this->chart = vtkSmartPointer<vtkChartXY>::New();
    this->view->GetRenderer()->SetBackground(1,1,1);
    this->view->GetScene()->AddItem(this->chart);
    this->renderWindow->Render();
    //暂时设置只添加一条线
    plot = this->chart ->AddPlot(vtkChart::LINE);
}

vtkLineChartWidget::~vtkLineChartWidget()
{
}

void vtkLineChartWidget::SetInputData(vtkMultiBlockDataSet* inputData)
{
    if (inputData == nullptr) {
        std::cerr << "Input data is nullptr." << std::endl;
        return;
    }
    this->multiBlock = inputData;
    this->unstructuredGrid = vtkUnstructuredGrid::SafeDownCast(this->multiBlock->GetBlock(0));
    vtkPointData* pointData = this->unstructuredGrid->GetPointData();

    int varNum = pointData->GetNumberOfArrays();
    QString tmp;
    std::string stdtmp;
    for(int i = 0; i < varNum; i++)
    {
        stdtmp = pointData->GetArrayName(i);
        std::cout << stdtmp << endl;
        tmp = QString::fromStdString(stdtmp);
        this->propertyList.append(tmp);

        vtkDataArray* dataArray = pointData->GetArray(i);
        this->table->AddColumn(dataArray);
    }

    this->dataStatus = true;
}

QList<QString> vtkLineChartWidget::GetPropertiesName()
{//默认数据是输入了的
    return this->propertyList;
}

bool vtkLineChartWidget::SetBottomAxisProperty(const QString& propertyName)
{
    if(!this->propertyList.contains(propertyName)){
        return false;
    }
    this->xID = this->propertyList.indexOf(propertyName);
    axisStatus[0] = true;
    return true;

}

bool vtkLineChartWidget::SetLeftAxisProperty(const QString& propertyName)
{
    if(!this->propertyList.contains(propertyName)){
        return false;
    }
    this->yID = this->propertyList.indexOf(propertyName);
    axisStatus[1] = true;
    return true;
}

void vtkLineChartWidget::SetBottomAxisTitle(const QString& title)
{
    this->chart->GetAxis(vtkAxis::BOTTOM)->SetTitle(title.toStdString());
    this->renderWindow->Render();
}
void vtkLineChartWidget::SetLeftAxisTitle(const QString& title)
{
    this->chart->GetAxis(vtkAxis::LEFT)->SetTitle(title.toStdString());
    this->renderWindow->Render();
}
void vtkLineChartWidget::SetChartTitle(const QString& title)
{
    this->chart->SetTitle(title.toStdString());
    this->renderWindow->Render();
}

bool vtkLineChartWidget::ShowLineChart()
{
    if(!axisStatus[0]||!axisStatus[1]) return false;
    this->plot->SetInputData(table,xID,yID);
    this->renderWindow->Render();
    return true;
}
