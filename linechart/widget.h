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
    void BackgroundButton_clicked();
    void SolidColorButton_clicked();
    void TButton_clicked();
    void XButton_clicked();
    void CutPlaneButton_clicked();
    void ColseCutWidget_clicked();
    void CutButton_clicked();
    void StreamSeedButton_clicked();
    void StreamTraceButton_clicked();

    void basicButton1_clicked();
    void basicButton2_clicked();
    void slice1Button1_clicked();
    void slice1Button2_clicked();
private:
    Ui::Widget *ui;
    vtkLineChartWidget* lineChartWidget;
};
#endif // WIDGET_H
