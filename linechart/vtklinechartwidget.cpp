#include "vtklinechartwidget.h"

vtkLineChartWidget::vtkLineChartWidget(QWidget *parent)
    :QVTKOpenGLNativeWidget(parent)
{
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
    this->view = vtkSmartPointer<vtkContextView>::New();
    this->renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    this->setRenderWindow(this->renderWindow);
    this->view->SetRenderWindow(this->renderWindow);
    //获取交互器
    this->qvtkInteractor = this->interactor();
    this->chart = vtkSmartPointer<vtkChartXY>::New();
    this->view->GetRenderer()->SetBackground(1,1,1);
    this->view->GetScene()->AddItem(this->chart);
    this->renderWindow->Render();
    this->firstLine = this->chart->AddPlot(vtkChart::LINE);
    this->chart->SetShowLegend(true);
    this->chart->GetLegend()->SetHorizontalAlignment(vtkChartLegend::RIGHT);
    this->chart->GetLegend()->SetVerticalAlignment(vtkChartLegend::TOP);
}

vtkLineChartWidget::~vtkLineChartWidget()
{
}

void vtkLineChartWidget::SetInputData(std::string fileName)
{
    auto reader = vtkSmartPointer<vtkDelimitedTextReader>::New();
    reader->SetFileName(fileName.c_str());
    reader->DetectNumericColumnsOn();  //对数值列自动检测
    reader->SetFieldDelimiterCharacters(" ");  //指定列之间的分隔符
    reader->SetHaveHeaders(true);
    reader->Update();  //执行读取操作，将文件内容读取到内存中，并准备好输出

    this->table = reader->GetOutput();
    int numColumns = table->GetNumberOfColumns();
    // 检查第一行是否是标题
    bool hasHeaders = false;
    std::string columnName = this->table->GetColumnName(0);
    if (!isdigit(columnName[0]))
    {
        hasHeaders = true;
    }
    if (!hasHeaders)
    {
        reader->SetHaveHeaders(false);
        reader->Update();
        this->table = reader->GetOutput();
        for (int i = 0; i < numColumns; i++)
        {
            std::string columnName = "Field" + std::to_string(i);
            table->GetColumn(i)->SetName(columnName.c_str());
            this->colunmsList.push_back(columnName.c_str());
        }
    }
    else
    {
        for (int i = 0; i < numColumns; i++)
        {
            const char* columnname = this->table->GetColumnName(i);
            this->colunmsList.push_back(columnname);
        }
    }
    this->dataStatus = true;
}

QList<QString> vtkLineChartWidget::GetColumnsName()
{//默认数据是输入了的
    return this->colunmsList;
}


void vtkLineChartWidget::SetBottomAxisTitle(const QString& title)
{
    this->chart->GetAxis(vtkAxis::BOTTOM)->SetTitle(title.toStdString());
    this->chart->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetFontSize(14);
    //this->renderWindow->Render();
}
void vtkLineChartWidget::SetLeftAxisTitle(const QString& title)
{
    this->chart->GetAxis(vtkAxis::LEFT)->SetTitle(title.toStdString());
    this->chart->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetFontSize(14);
    //this->renderWindow->Render();
}
void vtkLineChartWidget::SetChartTitle(const QString& title)
{
    this->chart->SetTitle(title.toStdString());
    this->chart->GetTitleProperties()->SetFontSize(16);
    //this->renderWindow->Render();
}

void vtkLineChartWidget::ShowLineChart()
{
    this->renderWindow->Render();
}

int vtkLineChartWidget::AddPlot(const std::string& xColumn, const std::string& yColumn, bool plotTypeIsLine)
{ //1是line，0是point
    this->lineNum++;
    if (plotTypeIsLine)
    {
        vtkPlot* line = this->chart->AddPlot(vtkChart::LINE);
        line->SetInputData(this->table, xColumn, yColumn);
        return lineNum;
    }
    else
    {
        vtkPlot* line = this->chart->AddPlot(vtkChart::POINTS);
        line->SetInputData(this->table, xColumn, yColumn);
        return lineNum;
    }
}

void vtkLineChartWidget::RemovePlot(int plotId)
{//id从1开始
    if (plotId >= 0 && plotId < this->chart->GetNumberOfPlots()) {
        this->chart->RemovePlot(plotId);
    }
}
