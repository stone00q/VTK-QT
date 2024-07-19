#ifndef VTKLINECHARTWIDGET_H
#define VTKLINECHARTWIDGET_H

#include <QWidget>

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
#include <vtkChartLegend.h>
#include <vtkRenderer.h>

#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKInteractor.h>
#include <QVTKOpenGLNativeWidget.h>

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingContextOpenGL2)
class vtkLineChartWidget:public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    vtkLineChartWidget(QWidget *parent = 0);
    ~vtkLineChartWidget();

    void SetInputData(std::string fileName);
    QList<QString> GetColumnsName();

    void SetBottomAxisTitle(const QString& title);
    void SetLeftAxisTitle(const QString& title);
    void SetChartTitle(const QString& title);

    int AddPlot(const std::string& xColumn, const std::string& yColumn, bool plotTypeIsLine);////1是line，0是point
    void RemovePlot(int plotId);
    void ShowLineChart();  //更新渲染效果

private:
    bool dataStatus = false; //2、3、4成员函数都得看下是否有设置输入
    int lineNum = 0;
    //不需要new的
    vtkPlot* firstLine;
    vtkSmartPointer<vtkTable> table;
    //需要new的
    vtkSmartPointer<vtkChartXY> chart;
    vtkSmartPointer<vtkContextView> view;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkSmartPointer<QVTKInteractor> qvtkInteractor;

    QList<QString> colunmsList;

};

#endif // VTKLINECHARTWIDGET_H
