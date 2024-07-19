#ifndef VTKLINECHARTWIDGET_H
#define VTKLINECHARTWIDGET_H

#include <QWidget>

#include <vtkMultiBlockDataSet.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkTable.h>
#include <vtkPlot.h>
#include <vtkChartXY.h>
#include <vtkContextView.h>
#include <vtkAxis.h>
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

    void SetInputData(vtkMultiBlockDataSet* inputData);
    QList<QString> GetPropertiesName();
    bool SetBottomAxisProperty(const QString& propertyName);  //x
    bool SetLeftAxisProperty(const QString& propertyName);  //y

    void SetBottomAxisTitle(const QString& title);
    void SetLeftAxisTitle(const QString& title);
    void SetChartTitle(const QString& title);

    bool ShowLineChart();

private:
    bool axisStatus[2] = {false};//0表示没有x、y设置，1表示设置了一个，2表示x、y都设置了
    bool dataStatus = false; //2、3、4成员函数都得看下是否有设置输入
    //不需要new的
    vtkMultiBlockDataSet* multiBlock;
    vtkUnstructuredGrid* unstructuredGrid;

    vtkPlot* plot;
    //需要new的
    vtkSmartPointer<vtkTable> table;
    vtkSmartPointer<vtkChartXY> chart;
    vtkSmartPointer<vtkContextView> view;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkSmartPointer<QVTKInteractor> qvtkInteractor;

    QList<QString> propertyList;
    int xID;
    int yID;

};

#endif // VTKLINECHARTWIDGET_H
