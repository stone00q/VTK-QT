#include "vtklinechartwidget.h"

vtkLineChartWidget::vtkLineChartWidget()
{
    this->view = vtkSmartPointer<vtkContextView>::New();
    this->renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    //获取交互器
    this->qvtkInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    this->qvtkInteractor->SetRenderWindow(this->renderWindow);
    this->view->SetRenderWindow(this->renderWindow);
    //this->table = vtkSmartPointer<vtkTable>::New();
    this->chart = vtkSmartPointer<vtkChartXY>::New();
    this->view->GetRenderer()->SetBackground(1, 1, 1);
    this->view->GetScene()->AddItem(this->chart);
    //this->renderWindow->Render();
    //qvtkInteractor->Start();
    //暂时设置只添加一条线
    this->firstLine = this->chart->AddPlot(vtkChart::LINE);
    this->propertyList.clear();
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
            this->propertyList.push_back(columnName);
        }
    }
    else
    {
        for (int i = 0; i < numColumns; i++)
        {
            const char* columnname = this->table->GetColumnName(i);
            this->propertyList.push_back(columnname);
        }
    }
    this->dataStatus = true;
}

std::vector<std::string> vtkLineChartWidget::GetPropertiesName()
{//默认数据是输入了的
    return this->propertyList;
}

//bool vtkLineChartWidget::SetBottomAxisProperty(const std::string& propertyName)
//{
//    int varNum = this->propertyList.size();
//    for (int i = 0; i < varNum; i++) 
//    {
//        if (this->propertyList[i] == propertyName) {
//            this->xID = i;
//            axisStatus[0] = true;
//            return true;
//        }
//    }
//    cout <<"选中的x轴变量id为："<< this->xID << endl;
//    return false;
//
//}
//
//bool vtkLineChartWidget::SetLeftAxisProperty(const std::string& propertyName)
//{
//    int varNum = this->propertyList.size();
//    for (int i = 0; i < varNum; i++)
//    {
//        if (this->propertyList[i] == propertyName) {
//            this->yID = i;
//            axisStatus[1] = true;
//            return true;
//        }
//    }
//    return false;
//}

void vtkLineChartWidget::SetBottomAxisTitle(const std::string& title)
{
    this->chart->GetAxis(vtkAxis::BOTTOM)->SetTitle(title);//报错了
    this->chart->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetFontSize(14);
    //this->renderWindow->Render();
}
void vtkLineChartWidget::SetLeftAxisTitle(const std::string& title)
{
    this->chart->GetAxis(vtkAxis::LEFT)->SetTitle(title);
    this->chart->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetFontSize(14);
    //this->renderWindow->Render();
}
void vtkLineChartWidget::SetChartTitle(const std::string& title)
{
    this->chart->SetTitle(title);
    this->chart->GetTitleProperties()->SetFontSize(16);
    //this->renderWindow->Render();
    //this->qvtkInteractor->Start();
}

void vtkLineChartWidget::ShowLineChart()
{
    //this->line = this->chart->AddPlot(vtkChart::LINE);
    //this->line->SetInputData(table,xID,yID);
    this->renderWindow->Render();
    this->qvtkInteractor->Start();
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
//bool vtkLineChartWidget::AddPlot(const std::string& xProperty, const std::string& yProperty, const std::string& plotType)
//{
//    int xIndex = -1, yIndex = -1;
//    for (int i = 0; i < propertyList.size(); ++i)
//    {
//        if (propertyList[i] == xProperty)
//            xIndex = i;
//        if (propertyList[i] == yProperty)
//            yIndex = i;
//    }
//
//    if (xIndex == -1 || yIndex == -1)
//    {
//        std::cerr << "Invalid property name." << std::endl;
//        return false;
//    }
//
//    vtkPlot* plot = nullptr;
//    if (plotType == "line")
//    {
//        plot = this->chart->AddPlot(vtkChart::LINE);
//    }
//    else if (plotType == "point")
//    {
//        plot = this->chart->AddPlot(vtkChart::POINTS);
//    }
//    else
//    {
//        std::cerr << "Invalid plot type specified." << std::endl;
//        return false;
//    }
//
//    plot->SetInputData(this->table, xIndex, yIndex);
//    plot->SetColor(rand() % 256, rand() % 256, rand() % 256, 255);
//    plot->SetWidth(2.0);
//
//    AddLegendEntry(plot, xProperty, yProperty);
//
//    return true;
//}

/*void vtkLineChartWidget::AddLegendEntry(vtkPlot* plot, const std::string& xProperty, const std::string& yProperty)
{
    std::string legendText = xProperty + " vs " + yProperty;

    // 获取plot的颜色
    unsigned char color[4];
    plot->GetPen()->GetColor(color);

    // 创建并设置TextProperty
    vtkSmartPointer<vtkTextProperty> textProp = vtkSmartPointer<vtkTextProperty>::New();
    textProp->SetColor(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);
    textProp->SetFontSize(12);
    textProp->SetBold(true);
    textProp->SetItalic(false);

    // 获取图例
    vtkSmartPointer<vtkChartLegend> legend = this->chart->GetLegend();
    if (!legend)
    {
        legend = vtkSmartPointer<vtkChartLegend>::New();
        this->chart->SetLegend(legend);
    }

    // 获取图例条目索引
    vtkIdType legendIndex = legend->GetNumberOfEntries();

    // 设置图例条目
    legend->SetEntry(legendIndex, plot, legendText.c_str());
    legend->SetEntryTextProperty(legendIndex, textProp);
}*/