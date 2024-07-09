#ifndef CLASS1_COLORMAP_H
#define CLASS1_COLORMAP_H

#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>
#include <vtkLookupTable.h>
#include <vtkScalarBarActor.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
class vtkColorMap {
public:
    // �ڹ��캯���г�ʼ��VTK��Ⱦ���ڣ���Ϊ�Ѿ�������Ⱦ���ڣ���δ����
    explicit vtkColorMap(vtkSmartPointer<vtkGenericOpenGLRenderWindow> existingRenderWindow = nullptr) {
        if (existingRenderWindow) {
            // ����ṩ�����е�RenderWindow����ʹ����
            this->renderWindow = existingRenderWindow;
        }
        else {
            // ���򣬴���һ���µ�RenderWindow
            this->renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
        }
        varNum = 0;
        selectedId = -1;
        mapper = vtkSmartPointer<vtkDataSetMapper>::New();
        actor = vtkSmartPointer<vtkActor>::New();
        lut = vtkSmartPointer<vtkLookupTable>::New();
        scalarBarActor = vtkSmartPointer<vtkScalarBarActor>::New();
        actor->SetMapper(mapper);
        //����ͼ��
       
        renderer = vtkSmartPointer<vtkRenderer>::New();
        renderer->AddActor(actor);
        renderer->AddActor(scalarBarActor);
        renderWindow->AddRenderer(renderer);
    }

    /***** ���ò��֣���Ҫ���ô����ug����+ѡ����ɫӳ��ı���*****/
    // ����Ĭ����unstructuredgrid��
    void setInputData(vtkUnstructuredGrid* inputData) {
        if (inputData == nullptr) {
            std::cerr << "Input data is nullptr." << std::endl;
            return;
        }
        this->unstructuredGrid = inputData;
        this->pointData = this->unstructuredGrid->GetPointData();
        varNum = pointData->GetNumberOfArrays();
    }
    // ���ý�����ɫӳ��ı������ݶ���������Ǳ���������������ת��Ϊ�ڼ�������
    void setSelectedVariable(std::string selectedVar) {
        for (int i = 0; i < varNum; i++) {
            if (pointData->GetArrayName(i) == selectedVar) {
                this->selectedId = i;
            }
        }
    }

    /*******���ز��֣����Ի����Ⱦ����*******/
    // ��ȡVTK��Ⱦ����
    vtkGenericOpenGLRenderWindow* getRenderWindow() {
        return this->renderWindow;
    }

    // ������Ⱦ
    void updateRenderWindow()
    {
        //���ñ�����������ɫӳ��
        scalars = pointData->GetArray(selectedId);  //ÿ��setscalr������֮ǰ��scalar
        unstructuredGrid->GetPointData()->SetScalars(scalars);
        //auto lut = vtkSmartPointer<vtkLookupTable>::New();
        lut->SetTableRange(scalars->GetRange());  //������ɫӳ�䷶Χ
        lut->SetNumberOfColors(256);
        //lut->SetHueRange(0.67, 0.0);
        lut->Build();

        //����ͼ��
        //auto scalarBarActor = vtkSmartPointer<vtkScalarBarActor>::New();
        scalarBarActor->SetLookupTable(lut);

        //auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
        mapper->SetInputData(unstructuredGrid);
        mapper->SetScalarRange(scalars->GetRange());
        mapper->SetLookupTable(lut);
        //mapper->SelectColorArray(selectedId);
        //mapper->SetScalarModeToUsePointFieldData();
        // auto actor = vtkSmartPointer<vtkActor>::New();
        // actor->SetMapper(mapper);
       
        //renderer->AddActor(actor);
        //renderer->AddActor(scalarBarActor);
        //renderWindow->AddRenderer(renderer);
        renderWindow->Render();//����Ҫ��Ҫд������
    }
    // �����ݶ��ر���Ⱦ����
    void closeRenderWindow() {
        if (this->renderWindow) {
            // ����Finalize��������ȷ�رմ���
            this->renderWindow->Finalize();

            // �ͷ�����Ⱦ������ص���Դ
            this->renderWindow = nullptr;
        }
    }

private:
    // ����һ��˽�б������洢unstructuredgrid����
    vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid;
    // �����������
    vtkSmartPointer<vtkPointData> pointData;
    vtkSmartPointer<vtkDataArray> scalars;
    vtkSmartPointer<vtkLookupTable> lut;
    // ��Ⱦ���
    vtkSmartPointer<vtkDataSetMapper> mapper;
    vtkSmartPointer<vtkScalarBarActor> scalarBarActor;
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    
    int varNum;
    int selectedId;
};



#endif
