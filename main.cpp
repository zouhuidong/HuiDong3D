#include <stdio.h>
#include "HuiDong3D.h"

#include <vector>
using namespace std;

/**
 * @brief		读取 VTK 文件的点
 * @param[in]	strFile: 文件路径
 * @param[out]	pNum: 读取到的点的数量
 * @param[in]	zoom: 读取到的点的缩放倍率
 * @return		返回读取到的点的数组
*/
Polygon3D* ReadVTK(const char* strFile, int* pNum, int zoom = 1000)
{
	FILE* fp;
	int r = fopen_s(&fp, strFile, "r");
	if (r != 0)
	{
		printf("Read vtk file error ( %d ).\n", r);
		return {};
	}

	// 读去多余的字符串
	char strTemp[1024] = { 0 };
	for (int i = 0; i < 4; i++)
		fgets(strTemp, 1024, fp);

	// 读点
	int nPointsNum = 0;
	fscanf_s(fp, "%s %d %s", strTemp, 1024, &nPointsNum, strTemp, 1024);

	ColorPoint3D* pPoints = new ColorPoint3D[nPointsNum];
	memset(pPoints, 0, sizeof ColorPoint3D * nPointsNum);

	for (int i = 0; i < nPointsNum; i++)
	{
		float x = 0, y = 0, z = 0;
		if (fscanf_s(fp, "%f %f %f", &x, &y, &z))
		{
			// 填白色
			//int grey = 255 - abs((int)(y * 800));
			//if (grey < 0) grey = 0;
			int grey = 255;
			Color c = RGB(grey, grey, grey);
			pPoints[i] = { x * zoom,y * zoom,z * zoom,c };
		}

		// read error
		else
		{
			printf("Error in reading points, have read %d points (%d all).\n", i, nPointsNum);
			return {};
		}
	}

	// 读多边形数据
	int nPolygonsNum = 0;
	fscanf_s(fp, "%s %d %s", strTemp, 1024, &nPolygonsNum, strTemp, 1024);

	// 三角形
	int nLinesNum = 3;

	Polygon3D* pPolygons = new Polygon3D[nPolygonsNum];
	for (int i = 0; i < nPolygonsNum; i++)
	{
		int a = 0, b = 0, c = 0;
		if (fscanf_s(fp, "%s %d %d %d", strTemp, 1024, &a, &b, &c))
		{
			pPolygons[i].nPointsNum = nLinesNum;
			pPolygons[i].pPoints[0] = pPoints[a];
			pPolygons[i].pPoints[1] = pPoints[b];
			pPolygons[i].pPoints[2] = pPoints[c];
			pPolygons[i].color =/* pPoints[a].color*/-1;
		}
		else
		{
			printf("Error in reading polygons, have read %d polygons (%d all).\n", i, nPolygonsNum);
			return pPolygons;
		}
	}

	delete[] pPoints;
	*pNum = nPolygonsNum;
	printf("Read %d points and %d polygons( %d lines for each polygon) of vtk file successfully.\n", nPointsNum, nPolygonsNum, nLinesNum);
	return pPolygons;
}

ColorPoint3D* ReadImageFile(LPCTSTR strFile, int* pNum)
{
	int w = 200, h = 200;

	IMAGE img(w, h);
	SetWorkingImage(&img);
	setlinecolor(BLUE);
	setfillcolor(WHITE);
	fillrectangle(0, 0, w, h);

	loadimage(&img, strFile, 200, 200);

	ColorPoint3D* pPoints = new ColorPoint3D[w * h];

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			pPoints[i * w + j] = { (double)i,(double)j,0,(Color)getpixel(i,j) };
		}
	}

	SetWorkingImage();

	*pNum = w * h;
	return pPoints;
}

Polygon3D* GetPillar()
{
	Polygon3D* pPolygons = new Polygon3D[6];
	Point3D pPoints[8] = {
		{0,300,0}, {200,300,0}, {200,300,200}, {0,300,200},
		{0,-100,200}, {200,-100,200}, {0,-100,0}, {200,-100,0}
	};

	pPolygons[0].nPointsNum = 4;
	pPolygons[0].pPoints[0] = pPoints[0];
	pPolygons[0].pPoints[1] = pPoints[1];
	pPolygons[0].pPoints[2] = pPoints[7];
	pPolygons[0].pPoints[3] = pPoints[6];
	pPolygons[0].color = GREEN;

	pPolygons[1].nPointsNum = 4;
	pPolygons[1].pPoints[0] = pPoints[0];
	pPolygons[1].pPoints[1] = pPoints[3];
	pPolygons[1].pPoints[2] = pPoints[4];
	pPolygons[1].pPoints[3] = pPoints[6];
	pPolygons[1].color = GREEN;

	pPolygons[2].nPointsNum = 4;
	pPolygons[2].pPoints[0] = pPoints[2];
	pPolygons[2].pPoints[1] = pPoints[3];
	pPolygons[2].pPoints[2] = pPoints[4];
	pPolygons[2].pPoints[3] = pPoints[5];
	pPolygons[2].color = GREEN;

	pPolygons[3].nPointsNum = 4;
	pPolygons[3].pPoints[0] = pPoints[1];
	pPolygons[3].pPoints[1] = pPoints[2];
	pPolygons[3].pPoints[2] = pPoints[5];
	pPolygons[3].pPoints[3] = pPoints[7];
	pPolygons[3].color = GREEN;

	pPolygons[4].nPointsNum = 4;
	pPolygons[4].pPoints[0] = pPoints[0];
	pPolygons[4].pPoints[1] = pPoints[1];
	pPolygons[4].pPoints[2] = pPoints[2];
	pPolygons[4].pPoints[3] = pPoints[3];
	pPolygons[4].color = RED;

	pPolygons[5].nPointsNum = 4;
	pPolygons[5].pPoints[0] = pPoints[4];
	pPolygons[5].pPoints[1] = pPoints[5];
	pPolygons[5].pPoints[2] = pPoints[7];
	pPolygons[5].pPoints[3] = pPoints[6];
	pPolygons[5].color = GREEN;

	return pPolygons;
}

ColorPoint3D* GetTextPoint3D(LPCTSTR str, int x, int y, int z, int* count)
{
	IMAGE img(textwidth(str), textheight(str));
	SetWorkingImage(&img);

	outtextxy(0, 0, str);

	// 上下翻转图像
	rotateimage(&img, &img, ConvertToRadian(180));

	// 左右翻转图像
	for (int i = 0; i < getwidth() / 2; i++)
	{
		for (int j = 0; j < getheight(); j++)
		{
			COLORREF c = getpixel(i,j);
			putpixel(i, j, getpixel(getwidth() - i, j));
			putpixel(getwidth() - i, j, c);
		}
	}

	ColorPoint3D* p = new ColorPoint3D[getwidth() * getheight()];
	int index = 0;
	for (int i = 0; i < getwidth(); i++)
	{
		for (int j = 0; j < getheight(); j++)
		{
			if (getpixel(i, j) != BLACK)
			{
				p[index] = { (double)x + i,(double)y + j,(double)z,WHITE };
				index++;
			}
		}
	}

	SetWorkingImage();

	*count = index;
	return &p[0];
}

int main()
{
	InitDrawingDevice(640, 480, 1);

	// read file
	Polygon3D* pPolygons = NULL;
	int nPolygonsNum = 0;
	//pPoints = ReadImageFile(L"./conan.png", &nPointsNum);
	pPolygons = ReadVTK("./fran_cut.vtk", &nPolygonsNum, 1000);

	///////  api

	// 3D 场景
	Scence3D scenceMain;

	ColorPoint3D pTest = { 10,20,30,GREEN };

	Object3D obj, obj2;
	obj.AddPolygons(pPolygons, nPolygonsNum);
	//obj.AddPolygons(pPolygons, nPolygonsNum);
	//obj.AddPoints(&pTest, 1);

	obj2.AddPoints(&pTest, 1);

	delete[] pPolygons;

	//scenceMain.AddObject(obj);
	//scenceMain.AddObject(obj2);

	Object3D objPillar;
	objPillar.AddPolygons(GetPillar(), 6);


	//致敬 ckj 的 flappy bird 中的柱子（特地将柱子顶面漆成了红色）
	int nTextPointsNum = 0;
	ColorPoint3D* pTextPoints = NULL;
	
	pTextPoints = GetTextPoint3D(L"致敬 ckj 的 flappy bird 项目", 10, 230, -10, &nTextPointsNum);
	objPillar.AddPoints(pTextPoints, nTextPointsNum);
	delete[] pTextPoints;

	pTextPoints = GetTextPoint3D(L"所以，我特地将柱子", 40, 200, -10, &nTextPointsNum);
	objPillar.AddPoints(pTextPoints, nTextPointsNum);
	delete[] pTextPoints;

	pTextPoints = GetTextPoint3D(L"顶面漆成了红色", 50, 180, -10, &nTextPointsNum);
	objPillar.AddPoints(pTextPoints, nTextPointsNum);
	delete[] pTextPoints;

	pTextPoints = GetTextPoint3D(L"by lovely_huidong~", 10, 120, -10, &nTextPointsNum);
	objPillar.AddPoints(pTextPoints, nTextPointsNum);
	delete[] pTextPoints;

	scenceMain.AddObject(objPillar);

	/*ColorPoint3D* pObjPoints = scenceMain.GetObjects()[index_obj1].GetPoints();
	int num = scenceMain.GetObjects()[index_obj1].GetPointsNum();
	for (int i = 0; i < num; i++)
	{
		if (pObjPoints[i].color != WHITE)
		{
			return -1;
		}
	}*/

	BeginBatchDraw();

	ExMessage msg;
	int old_x = -1, old_y = -1;

	while (true)
	{
		double fps = 1.0 / scenceMain.Draw(-100, 100, WHITE);

		wchar_t str[32] = { 0 };
		wsprintf(str, L"fps: %d", (int)fps);
		outtextxy(0, 0, str);

		// temp
		/*FlushBatchDraw();
		cleardevice();
		scenceMain.GetObjects()[0]->RotateX(1);
		scenceMain.GetObjects()[0]->UpdateRotatePoints();
		continue;*/

		msg = getmessage(EM_MOUSE | EM_KEY);
		if (msg.lbutton)
		{
			if (old_x != -1)
			{
				scenceMain.GetObjects()[0].RotateY(-(old_x - msg.x));
				scenceMain.GetObjects()[0].RotateX(-(msg.y - old_y));
				scenceMain.GetObjects()[0].UpdateRotatedPoints();
			}

			old_x = msg.x;
			old_y = msg.y;
		}
		else
		{
			old_x = old_y = -1;
		}

		if (msg.vkcode == 'R')
		{
			scenceMain.GetObjects()[0].SetAttitude({ 0,0,0 });
			scenceMain.GetObjects()[0].UpdateRotatedPoints();
			printf("reset\n");
		}

		flushmessage(EM_MOUSE);

		FlushBatchDraw();
		cleardevice();
	}

	getmessage(EM_CHAR);
	CloseDrawingDevice();
	return 0;
}
