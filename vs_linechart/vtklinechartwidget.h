#ifndef VTKLINECHARTWIDGET_H
#define VTKLINECHARTWIDGET_H

#include <vtkDelimitedTextReader.h>
#include <vtkDoubleArray.h>
#include <vtkTable.h>
#include <vtkPlot.h>
#include <vtkChartXY.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkPen.h>
#include <vtkAxis.h>
#include <vtkTextProperty.h>

#include <vtkRenderer.h>

#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

//#include <vtkGenericOpenGLRenderWindow.h>



class vtkLineChartWidget
{
public:
    vtkLineChartWidget();
    ~vtkLineChartWidget();

    void SetInputData(std::string fileName);
    std::vector<std::string> GetPropertiesName();
    //bool SetBottomAxisProperty(const std::string& propertyName);  //x
    //bool SetLeftAxisProperty(const std::string& propertyName);  //y

    void SetBottomAxisTitle(const std::string& title);
    void SetLeftAxisTitle(const std::string& title);
    void SetChartTitle(const std::string& title);

    int AddPlot(const std::string& xColumn, const std::string& yColumn, bool plotTypeIsLine);////1是line，0是point
    void RemovePlot(int plotId);
    void ShowLineChart();  //更新渲染效果
private:
    //bool axisStatus[2] = {false};//0表示没有x、y设置，1表示设置了一个，2表示x、y都设置了
    bool dataStatus = false; //2、3、4成员函数都得看下是否有设置输入
    std::map<std::string, vtkPlot*> linesList;
    //不需要new的
    vtkPlot* firstLine;
    vtkSmartPointer<vtkTable> table;
    //需要new的
    vtkSmartPointer<vtkChartXY> chart;
    vtkSmartPointer<vtkContextView> view;
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> qvtkInteractor;

    std::vector<std::string> propertyList;
    int lineNum = 0;
    /*int xID;
    int yID;*/
    void AddLegendEntry(vtkPlot* plot, const std::string& xProperty, const std::string& yProperty);
};

#endif // VTKLINECHARTWIDGET_H
