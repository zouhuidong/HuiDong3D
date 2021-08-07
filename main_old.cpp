#include <stdio.h>
#include "HuiDong3D.h"

/**
 * @brief		��ȡ VTK �ļ��ĵ�
 * @param[in]	strFile: �ļ�·��
 * @param[out]	pNum: ��ȡ���ĵ������
 * @return		���ض�ȡ���ĵ������
 */
ColorPoint3D* ReadVTK(const char* strFile, int* pNum, int zoom = 1000)
{
	FILE* fp;
	int r = fopen_s(&fp, strFile, "r");
	if (r != 0)
	{
		printf("Read vtk file error ( %d ).\n", r);
		return NULL;
	}

	// ��ȥ������ַ���
	char strTemp[1024] = { 0 };
	for (int i = 0; i < 4; i++)
		fgets(strTemp,1024,fp);

	// ����
	int nPointsNum = 0;
	fscanf_s(fp, "%s %d %s", strTemp, 1024, &nPointsNum, strTemp, 1024);

	ColorPoint3D* pPoints = new ColorPoint3D[nPointsNum];
	memset(pPoints, 0, sizeof ColorPoint3D * nPointsNum);

	for (int i = 0; i < nPointsNum; i++)
	{
		float x = 0, y = 0, z = 0;
		if (fscanf_s(fp, "%f %f %f", &x, &y, &z))
		{
			// ���ɫ
			int grey = 255 - y*1200;
			//if (grey < 0) grey = 0;
			Color c = RGB(grey, grey, grey);
			pPoints[i] = { x * zoom,y * zoom,z * zoom,c };
		}

		// read error
		else
		{
			*pNum = i;
			printf("Error in reading points, have read %d points (%d all).\n", i, nPointsNum);
			return pPoints;
		}
	}

	// �����������
	int nPolygonsNum = 0;
	fscanf_s(fp, "%s %d %s", strTemp, 1024, &nPolygonsNum, strTemp, 1024);

	// ������
	int nLinesNum = 3;

	ColorPoint3D* pPolygons = new ColorPoint3D[nPolygonsNum * nLinesNum];
	memset(pPolygons, 0, sizeof ColorPoint3D * nPolygonsNum * nLinesNum);

	for (int i = 0; i < nPolygonsNum; i++)
	{
		int a = 0, b = 0, c = 0;
		if (fscanf_s(fp, "%s %d %d %d", strTemp, 1024, &a, &b, &c))
		{
			pPolygons[i * nLinesNum] = pPoints[a];
			pPolygons[i * nLinesNum + 1] = pPoints[b];
			pPolygons[i * nLinesNum + 2] = pPoints[c];
		}
		else
		{
			*pNum = i * nLinesNum;
			printf("Error in reading polygons, have read %d polygons (%d all).\n", i, nPolygonsNum);
			return pPolygons;
		}
	}

	delete[] pPoints;
	*pNum = nPolygonsNum * nLinesNum;
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
	ColorPoint3D* pPoints = NULL;
	int nPointsNum = 0;
	//pPoints = ReadImageFile(L"./conan.png", &nPointsNum);
	pPoints = ReadVTK("./fran_cut.vtk", &nPointsNum, 1000);

	///////  api

	// 3D ����
	Scence3D scenceMain;

	Object3D obj;
	obj.AddPoints(pPoints, nPointsNum);
	delete[] pPoints;
	scenceMain.AddObject(obj);

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
				scenceMain.GetObjects()[0]->RotateY(msg.x - old_x);
				scenceMain.GetObjects()[0]->RotateX(msg.y - old_y);
				scenceMain.GetObjects()[0]->UpdateRotatePoints();
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
			scenceMain.GetObjects()[0]->SetAttitude({ 0,0,0 });
			scenceMain.GetObjects()[0]->UpdateRotatePoints();
			printf("reset\n");
		}

		flushmessage(EM_MOUSE);

		FlushBatchDraw();
		cleardevice();
	}


	///////  independence


	//ColorPoint3D* pRotatePoints = new ColorPoint3D[w * h];
	//memset(pRotatePoints, 0, sizeof ColorPoint3D * w * h);

	//Point3D center = GetCenter(pPoints, w * h);
	////center = { 0,0,0 };

	//double a = 0;
	//double e = 0;
	//double r = 0;

	//BeginBatchDraw();

	//ExMessage msg;
	//int old_x = -1, old_y = -1;

	//while (true)
	//{
	//	int t = clock();

	//	for (int i = 0; i < w * h; i++)
	//	{
	//		pRotatePoints[i] = Rotate3DPoint(pPoints[i], a, e, r, center);
	//		setimagebuffer((int)pRotatePoints[i].x, (int)pRotatePoints[i].y, pRotatePoints[i].color, 200, 200);
	//	}

	//	msg = getmessage(EM_MOUSE | EM_KEY);
	//	if (msg.lbutton)
	//	{
	//		if (old_x != -1)
	//		{
	//			e += (msg.x - old_x);
	//			r += (msg.y - old_y);
	//		}

	//		old_x = msg.x;
	//		old_y = msg.y;
	//	}
	//	else
	//	{
	//		old_x = old_y = -1;
	//	}

	//	if (msg.vkcode == 'R')
	//	{
	//		a = 0;
	//		e = 0;
	//		r = 0;
	//		printf("reset\n");
	//	}

	//	flushmessage(EM_MOUSE);

	//	int fps = (int)(1.0 / ((double)(clock() - t) / CLOCKS_PER_SEC));
	//	if (clock() - t == 0) fps = 9999;	// ֡�ʹ���

	//	wchar_t str[32] = { 0 };
	//	wsprintf(str, L"fps: %d", fps);
	//	outtextxy(0, 0, str);

	//	FlushBatchDraw();
	//	cleardevice();

	//	//Sleep(10);
	//}


	/////////////////////  close  //////////////////////

	getmessage(EM_CHAR);
	CloseDrawingDevice();
	return 0;
}