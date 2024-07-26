#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>
#include "vtkgeometrywidget.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void StepButton_clicked();
    void IgesButton_clicked();
private:
    Ui::Widget *ui;
    vtkGeometryWidget* geometryWidget;
};
#endif // WIDGET_H
