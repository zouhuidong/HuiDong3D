#include <stdio.h>
#include "HuiDong3D.h"

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
		fgets(strTemp,1024,fp);

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
			int grey = 255 - abs((int)(y * 800));
			//if (grey < 0) grey = 0;
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
			pPolygons[i].color = pPoints[i].color;
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
	obj.AddPolygons(pPolygons, nPolygonsNum);
	obj.AddPoints(&pTest, 1);

	obj2.AddPoints(&pTest, 1);

	scenceMain.AddObject(obj);
	scenceMain.AddObject(obj2);

	BeginBatchDraw();

	ExMessage msg;
	int old_x = -1, old_y = -1;

	while (true)
	{
		double fps = 1.0 / scenceMain.Draw(300, 300);

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
				scenceMain.GetObjects()[0].RotateY(msg.x - old_x);
				scenceMain.GetObjects()[0].RotateX(msg.y - old_y);
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
