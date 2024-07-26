#ifndef VTKGEOMETRYWIDGET_H
#define VTKGEOMETRYWIDGET_H
#include <QWidget>

#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkCompositePolyDataMapper.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkNew.h>
#include <vtkRegressionTestImage.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTestUtilities.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include "vtkOCCTReader.h"


#include "vtkOCCTReader.h"

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkInteractionStyle)
class vtkGeometryWidget:public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    explicit vtkGeometryWidget(QWidget *parent = 0);
    ~vtkGeometryWidget();
    void SetInputData(QString fileName);
private:
    vtkSmartPointer<vtkOCCTReader> geoReader;

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkSmartPointer<QVTKInteractor> qvtkInteractor;
};

#endif // VTKGEOMETRYWIDGET_H
