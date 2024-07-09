#ifndef VTKTECPLOTWIDGET_H
#define VTKTECPLOTWIDGET_H

#include <QWidget>

#include <vtkMultiBlockDataSet.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>

#include <vtkProperty.h>
#include <vtkLookupTable.h>
#include <vtkScalarBarActor.h>

#include <vtkImplicitPlaneWidget2.h>
#include <vtkImplicitPlaneRepresentation.h>
#include <vtkCutter.h>
#include <vtkPlane.h>
#include <vtkPolyDataMapper.h>

#include <vtkArrayCalculator.h>
#include <vtkSphereWidget.h>
#include <vtkNamedColors.h>
#include <vtkPointSource.h>
#include <vtkStreamTracer.h>
#include <vtkRungeKutta4.h>


#include <QVTKOpenGLNativeWidget.h>

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
class vtkTecplotWidget:public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    explicit vtkTecplotWidget(QWidget *parent = 0);
    ~vtkTecplotWidget();

    void SetInputData(vtkMultiBlockDataSet* inputData);

    int GetPropertiesNumber();
    std::string GetPropertyName(int id);

    /***设置、获取渲染器背景颜色***/
    void SetBackgroundColor(QColor color = QColorConstants::DarkCyan);
    QColor GetBackgroundColor();

    /***设置basicActor、basicMapper的颜色***/
    void SetSolidColor(QColor  RGBA = QColorConstants::LightGray);
    QColor GetSolidColor();
    void SetSolidVisible(bool flag);

    /***设置颜色映射变量***/
    void SetColorMapVariable(std::string selectedVar);
    void SetColorMappingFlag(bool flag = false);

    //Cut
    void SetCutPlane();
    void SetCutApply();
    void SetCutPlaneFlag(bool flag=true);
    bool GetCutPlaneVisiable();


    bool AddCutPlane(QString name = "cutPlane1" );
    bool SetCutPlaneVisable(QString name, bool flag=true);
    bool DelCutPlane(QString name);
    void ShowCutPlaneWidget(QString name, bool flag=true);


    //void SetCutPlane(QPoint origin, QVector normal);
    //QPoint GetCutOrigin();
    //QVector GetCutNormal();
    void SetCutColorMapVariable(std::string varName);

    //StreamLine
    void SetStreamTrancerSeed(int PointNum=100);
    void SetStreamTrancerApply();

    bool SetVelocity(QString u, QString v, QString w);
    bool SetVelocity(QString vel);

private:

    //共享
    vtkMultiBlockDataSet* multiBlock;
    vtkUnstructuredGrid* unstructuredGrid;
    vtkPointData* pointData;
    vtkDataArray* scalars;

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

    vtkSmartPointer<QVTKInteractor> qvtkInteractor;

    //截面的法平面
    //vtkSmartPointer<vtkPlaneWidget> cutPlaneWidget;
    vtkSmartPointer<vtkImplicitPlaneWidget2> cutPlaneWidget;
    vtkSmartPointer<vtkImplicitPlaneRepresentation> cutPlaneRep;

    //流线
    vtkSmartPointer<vtkSphereWidget> seedSphereWidget;
    //渲染管理actor
    std::map<std::string,vtkDataSetMapper*> mappersMap;
    std::map<std::string,vtkActor*> actorsMap;
    std::map<std::string,vtkLookupTable*> lutsMap;
    std::map<std::string,vtkScalarBarActor*> barActorsMap;

    int varNum;
    int seedPointNum;




};

#endif // VTKTECPLOTWIDGET_H
