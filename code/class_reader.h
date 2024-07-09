#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>

#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkFloatArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkIdList.h>
#include <vtkPointData.h>
#include <vtkMultiBlockDataSet.h>

#include <ctime> 

#include <vtkArrayCalculator.h>

class TecplotReader {
private:
    std::string fileName;

    // Helper functions:
    void pointsReader(int pointId, const std::string& line, int varNum, std::vector<vtkSmartPointer<vtkFloatArray>>& zoneData, vtkPoints* thePoints);
    void cellsReader(const std::string& cellType, const std::string& line, vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid);

public:
    TecplotReader(const std::string& fileName) : fileName(fileName) {}
    vtkMultiBlockDataSet* readTecplot();
};

void TecplotReader::pointsReader(int pointId, const std::string& line, int varNum, std::vector<vtkSmartPointer<vtkFloatArray>>& zoneData, vtkPoints* thePoints) {
	std::istringstream iss(line);
	std::string theValue;
	//std::cout << std::endl << "该点是" <<  std::endl;
	float x, y, z, tmp;
	for (int i = 0; i < varNum; i++)
	{
		iss >> theValue;
		tmp = stof(theValue);
		//std::cout << tmp << " ";
		zoneData[i]->SetValue(pointId, tmp);
		switch (i)
		{
		case 0:
			x = tmp;
			break;
		case 1:
			y = tmp;
			break;
		case 2:
			z = tmp;
			break;
		default:
			break;
		}
	}
	thePoints->InsertNextPoint(x, y, z);
}

void TecplotReader::cellsReader(const std::string& cellType, const std::string& line, vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid) {
	std::istringstream iss(line);
	std::string tmp;
	std::vector<int> realcell;
	auto ids = vtkSmartPointer<vtkIdList>::New();
	int cur = 0;
	while (iss >> tmp)
	{
		int theID = stoi(tmp) - 1; //给出的数据索引从1开始，但是vtk从0开始

		if (realcell.empty())
		{
			realcell.push_back(theID);
			ids->InsertNextId(theID);
		}
		else if (theID != realcell[cur]) {
			realcell.push_back(theID);
			cur++;
			ids->InsertNextId(theID);
		}
		//std::cout << "theID : " << theID << std::endl;
	}
	int len = realcell.size();
	switch (len)
	{
	case 3:   //三角形
		unstructuredGrid->InsertNextCell(VTK_TRIANGLE, ids);
		break;
	case 4:   //四边形or四面体
		if (cellType == "FEQUADRILATETAL")
			unstructuredGrid->InsertNextCell(VTK_QUAD, ids);
		else unstructuredGrid->InsertNextCell(VTK_TETRA, ids);
		break;
	case 5:  //金字塔
		unstructuredGrid->InsertNextCell(VTK_PYRAMID, ids);
		break;
	case 6:  //三棱柱
		unstructuredGrid->InsertNextCell(VTK_WEDGE, ids);
		break;
	case 8:  //六面体
		unstructuredGrid->InsertNextCell(VTK_HEXAHEDRON, ids);
		break;
	default:
		break;
	}
}

vtkMultiBlockDataSet* TecplotReader::readTecplot() {
	clock_t start_time = clock();   //获取开始执行时间

	std::ifstream file(this->fileName); //打开文件
	std::string line;
	std::string currentToken;

	std::string title;  //整个文件名
	int solutiontime = 0;
	int zoneNum = 0;
	int nodeNum; //the number of points

	std::vector<std::string> varName;  //变量名

	std::vector<std::string> zoneTitle; //存放zone的名称
	std::vector<int> zoneCellNum;  //区域网格数量
	std::vector<std::string> zoneCellType;  //zoneType网格结构


	auto thePoints = vtkSmartPointer<vtkPoints>::New();
	std::vector<vtkSmartPointer<vtkFloatArray>> zoneData;//存放所有点的信息（包括坐标和属性
	//auto pointsData = vtkSmartPointer<vtkPoints>::New();
	vtkMultiBlockDataSet* multiBlock = vtkMultiBlockDataSet::New();
	auto sharedPointData = vtkSmartPointer<vtkPointData>::New();

	/*
	??这样简化不知道是否可以
	每个zone读入，当zoneNum=0，即第一个区域读入，只有第一个zone需要读入点和solutiontime、文件title
	其他zone只需要读入结构,即zone一行后紧接着结构数据
	*/

	bool pointsOK = false;
	while (std::getline(file, line))  //逐行读取文件 
	{
		if (line.empty()) continue;  //空行跳过
		if (!pointsOK)
		{
			if (line.find("TITLE") != std::string::npos)
			{
				int pos1 = line.find("\"");
				int pos2 = line.find("\"", pos1 + 1);
				title = line.substr(pos1 + 1, pos2 - pos1 - 1);
				//std::cout << title << std::endl;
				continue;
			}

			if (line.find("VARIABLES") != std::string::npos)
			{
				size_t pos1 = line.find("\"");
				size_t pos2 = line.find("\"", pos1 + 1);
				std::string tempVarName;
				while (pos2 != std::string::npos) {
					tempVarName = line.substr(pos1 + 1, pos2 - pos1 - 1);
					//std::cout << tempVarName << std::endl;
					varName.push_back(tempVarName);
					if (pos2 == line.size() - 1) break;  //??不加这个好像就一直在while循环里了
					pos1 = line.find("\"", pos2 + 1);
					pos2 = line.find("\"", pos1 + 1);
				}
				continue;
			}

			if (line.find("solutiontime") != std::string::npos)
			{
				std::istringstream iss(line);  //创建字符串流，用于分割单词：以空格、制表符为结束
				while (iss >> currentToken) {
					if (std::isdigit(currentToken[0]))
					{
						solutiontime = std::stoi(currentToken);
						break;
					}
				}
				continue; //
			}

			if (line.find("ZONE") != std::string::npos)
			{//进入zone信息行，包括点数、cell数和cell类型，然后就读入点
				//等于号和逗号都替换为空格，就可以关键词后面直接跟着值
				zoneNum++;
				std::replace(line.begin(), line.end(), '=', ' ');
				std::replace(line.begin(), line.end(), ',', ' ');
				std::replace(line.begin(), line.end(), '"', ' ');
				std::istringstream iss(line);  //创建字符串流，用于分割单词：以空格、制表符为结束

				while (iss >> currentToken)
				{
					if (currentToken == "T")
					{
						iss >> currentToken; //T的下一个token，即值
						//std::cout <<"T=" <<currentToken << std::endl;
						zoneTitle.push_back(currentToken);
					}
					else if (currentToken == "N")
					{
						iss >> nodeNum;
						//std::cout <<"N="<< nodeNum << std::endl;
					}
					else if (currentToken == "E")
					{
						iss >> currentToken;
						//std::cout <<"E="<< currentToken << std::endl;
						zoneCellNum.push_back(std::stoi(currentToken));
					}
					else if (currentToken == "ZONETYPE")
					{
						iss >> currentToken;
						zoneCellType.push_back(currentToken);
						break;
					}
				}
				continue;
			}
			//??非空、不是信息行且还在第一个zone，默认这一条是点数据
			int varNum = varName.size();
			//空间分配
			for (int i = 0; i < varNum; i++) {
				vtkFloatArray* theArray = vtkFloatArray::New();
				theArray->SetNumberOfTuples(nodeNum);  //tuples指的是含有元素的个数，components指的是每个元素用几个元组成
				theArray->SetName(varName[i].c_str());
				zoneData.push_back(theArray);
				theArray->Delete();
			}
			pointsReader(0, line, varNum, zoneData, thePoints);

			//2-N行读取
			for (int i = 1; i < nodeNum; i++)
			{
				std::getline(file, line);
				pointsReader(i, line, varNum, zoneData, thePoints);
			}
			pointsOK = true;

			//新增加的部分，如果points全部读入，就新设置速度变量
			if (pointsOK)
			{
			}
			continue;
		}

		if (line.find("ZONE") != std::string::npos)
		{//进入zone信息行，包括点数、cell数和cell类型，然后就读入点
			//等于号和逗号都替换为空格，就可以关键词后面直接跟着值
			zoneNum++;

			std::replace(line.begin(), line.end(), '=', ' ');
			std::replace(line.begin(), line.end(), '"', ' ');
			std::replace(line.begin(), line.end(), ',', ' ');
			std::istringstream iss(line);  //创建字符串流，用于分割单词：以空格、制表符为结束
			//std::cout << line << std::endl;
			while (iss >> currentToken)
			{
				if (currentToken == "T")
				{
					iss >> currentToken; //T的下一个token，即值
					//std::cout << currentToken << std::endl;
					zoneTitle.push_back(currentToken);
				}
				else if (currentToken == "E")
				{
					iss >> currentToken;
					//std::cout << currentToken << std::endl;
					zoneCellNum.push_back(std::stoi(currentToken));
				}
				else if (currentToken == "ZONETYPE")
				{
					iss >> currentToken;
					zoneCellType.push_back(currentToken);
					break;
				}
			}
			continue;
		}
		//以上情况都不是，那就是zone的cell第一行
		auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
		ug->SetPoints(thePoints);

		//只在第一个zone区域，将u、v、w整合成
		if (zoneNum == 1) {
			for (auto& data : zoneData) {
				ug->GetPointData()->AddArray(data);
			}
			auto calculator = vtkSmartPointer<vtkArrayCalculator>::New();
			calculator->SetInputData(ug);
			calculator->AddScalarArrayName("u");
			calculator->AddScalarArrayName("v");
			calculator->AddScalarArrayName("w");
			calculator->SetResultArrayName("velocity");
			calculator->SetFunction("u*iHat + v*jHat + w*kHat");
			calculator->Update();

			ug = calculator->GetUnstructuredGridOutput();
			sharedPointData = ug->GetPointData();
		}
		else {
			ug->GetPointData()->ShallowCopy(sharedPointData);
		}
		
		int zoneId = zoneNum - 1;
		//std::cout << "zoneId = " << zoneId << std::endl;
		cellsReader(zoneCellType[zoneId], line, ug);
		for (int i = 1; i < zoneCellNum[zoneId]; i++)
		{
			std::getline(file, line);
			cellsReader(zoneCellType[zoneId], line, ug);
		}
		multiBlock->SetBlock(zoneId, ug);
	}

	clock_t end_time = clock();     //获取结束时间
	std::cout << "数据读入时间：" << (end_time - start_time) / (double)CLOCKS_PER_SEC << "s" << std::endl;

	return multiBlock;
}