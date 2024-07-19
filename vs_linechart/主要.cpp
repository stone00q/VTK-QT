#include "class_reader.h"
#include "vtklinechartwidget.h"

#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingContextOpenGL2)
int main() {
	vtkLineChartWidget lineChart;
	//lineChart.SetInputData(R"(D:\Project\VTK_QT\data\test_untitled.csv)");
	lineChart.SetInputData(R"(D:\Project\VTK_QT\data\test.csv)");
	//�鿴�б���û������
	std::vector<std::string> propertyList = lineChart.GetPropertiesName();
	cout << endl << "��ѡ�������£�"<<endl;
	for (auto name : propertyList)
	{
		cout << name << " ";
	}

	std::string xName1, yName1,xName2,yName2;
	std::string xAxis, yAxis,chartName;
	cout << endl << "�������һ��line��x��������y������" << endl;
	cin >> xName1 >> yName1;
	cout << endl << "������ڶ���line��x��������y������" << endl;
	cin >> xName2 >> yName2;
	cout << endl << "x��������" << endl;
	cin >> xAxis;
	cout << endl << "ͼ��������" << endl;
	cin >> chartName;
	//cout << endl << "y��������" << endl;
	//cin >> yAxis;
	lineChart.SetBottomAxisTitle(xAxis);
	lineChart.SetChartTitle(chartName);
	int id1 = lineChart.AddPlot(xName1, yName1, 0);
	int id2 = lineChart.AddPlot(xName2, yName2, 1);
	//lineChart.RemovePlot(id2);
	lineChart.ShowLineChart();
	/************************************
	*************���濪ʼ��Ⱦ************
	***********************************
	auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	auto renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(renderer);

	auto renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	auto ug = vtkUnstructuredGrid::SafeDownCast(multiBlock->GetBlock(0));
	auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputData(ug);

	auto actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	renderer->AddActor(actor);
	for (int i = 0; i < multiBlock->GetNumberOfBlocks(); i++) {
		vtkSmartPointer<vtkUnstructuredGrid> ug = vtkUnstructuredGrid::SafeDownCast(multiBlock->GetBlock(i));
		auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		mapper->SetInputData(ug);

		auto actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);

		auto renderer = vtkSmartPointer<vtkRenderer>::New();
		renderer->AddActor(actor);

		renderWindow->AddRenderer(renderer);
	}

	renderWindow->Render();
	renderWindowInteractor->Start();
	
	
	
	chat���Ĳ��Դ���
	vtkLineChartWidget chartWidget;
    chartWidget.SetInputData("data.csv"); // ����data.csv�ļ������Ҹ�ʽ��ȷ
    chartWidget.SetBottomAxisProperty("Time");
    chartWidget.SetLeftAxisProperty("Sensor1");
    chartWidget.AddPlot("Time", "Sensor1", "line");
    chartWidget.AddPlot("Time", "Sensor2", "point");
    chartWidget.SetBottomAxisTitle("Time (s)");
    chartWidget.SetLeftAxisTitle("Sensor Readings");
    chartWidget.SetChartTitle("Sensor Data Over Time");
    chartWidget.ShowLineChart();
	*/

	return 0;
}