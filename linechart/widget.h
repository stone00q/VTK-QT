#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QProcess>
#include "vtklinechartwidget.h"
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
    void addFirstPlot_clicked();
private:
    Ui::Widget *ui;
    vtkLineChartWidget* lineChartWidget;
};
#endif // WIDGET_H
