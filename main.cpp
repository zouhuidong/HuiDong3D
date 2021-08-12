#include <stdio.h>
#include "HuiDong3D.h"
using namespace HD3D;

#include <vector>
using namespace std;

/**
 * @brief		��ȡ VTK �ļ��ĵ�
 * @param[in]	strFile: �ļ�·��
 * @param[out]	pNum: ��ȡ���ĵ������
 * @param[in]	zoom: ��ȡ���ĵ�����ű���
 * @return		���ض�ȡ���ĵ������
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

	// ��ȥ������ַ���
	char strTemp[1024] = { 0 };
	for (int i = 0; i < 4; i++)
		fgets(strTemp, 1024, fp);

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
			int grey = 255 - ((int)((y * 12 + z * 3) * 80));
			//if (grey < 0) grey = 0;
			//int grey = 255;
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

	// �����������
	int nPolygonsNum = 0;
	fscanf_s(fp, "%s %d %s", strTemp, 1024, &nPolygonsNum, strTemp, 1024);

	// ������
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
			//pPolygons[i].color = pPoints[a].color;
			pPolygons[i].color = -1;
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

// ��ȡһ�� 3D ���ӵĶ���μ���
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

	// ���·�תͼ��
	rotateimage(&img, &img, ConvertToRadian(180));

	// ���ҷ�תͼ��
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

// ��ȡ 3D ��������
Object3D* GetPillarObject()
{
	Object3D* objPillar = new Object3D;
	objPillar->AddPolygons(GetPillar(), 6);

	// ����
	int nTextPointsNum = 0;
	ColorPoint3D* pTextPoints = NULL;

	/*pTextPoints = GetTextPoint3D(L"������Ӳ���", 10, 230, -10, &nTextPointsNum);
	objPillar.AddPoints(pTextPoints, nTextPointsNum);
	delete[] pTextPoints;

	pTextPoints = GetTextPoint3D(L"�ڶ�������", 40, 200, -10, &nTextPointsNum);
	objPillar.AddPoints(pTextPoints, nTextPointsNum);
	delete[] pTextPoints;*/
	
	pTextPoints = GetTextPoint3D(GetEasyXVer(), 10, 230, -1, &nTextPointsNum);
	objPillar->AddPoints(pTextPoints, nTextPointsNum);
	delete[] pTextPoints;

	return objPillar;
}

// ��ȡ 3D ģ������
Object3D* GetModelObject()
{
	Object3D* obj = new Object3D;

	// read file

	Polygon3D* pPolygons = NULL;
	int nPolygonsNum = 0;
	pPolygons = ReadVTK("./fran_cut.vtk", &nPolygonsNum, 1400);
	//pPolygons = ReadVTK("./bunny.vtk", &nPolygonsNum, 1400);
	obj->AddPolygons(pPolygons, nPolygonsNum);

	/*ColorPoint3D* pPoints = NULL;
	int nPointsNum = 0;
	pPoints = ReadImageFile(L"./conan.png", &nPointsNum);
	obj->AddPoints(pPoints, nPointsNum);*/

	return obj;
}

int main()
{
	// ��ʼ����ͼ�豸
	InitDrawingDevice(640, 480, 1);

	// ���û�ͼ�豸����ɫ
	setbkcolor(RGB(130, 190, 230));
	cleardevice();

	// 3D ����
	Scence3D scenceMain;
	Scence3D scence2;

	// ��������
	scenceMain.AddObject(*GetPillarObject());
	//scenceMain.AddObject(*GetModelObject());

	// �ƶ�����
	scenceMain.GetObjects()[0].MoveTo({ 0,0,100});

	// �����������
	//scenceMain.SetCameraFocalLength(1000);

	// ���� / ���� ͸��ͶӰ
	scenceMain.EnablePerspectiveProjection(true);

	// �ڶ�������
	scence2.AddObject(*GetModelObject());

	// ��ʼ������ͼ
	BeginBatchDraw();

	// ��ȡ�����Ϣ
	ExMessage msg;
	int old_x = -1, old_y = -1;

	// ��ǰ�������ָ��
	Scence3D* pScence = &scenceMain;

	// ��Ϣ��ѭ��
	while (true)
	{
		// ���ó�����Ⱦ��������ȡ��Ⱦʱ��
		double fps = 1.0 / pScence->Render(-300, -200, { 0.6,0.6 }, WHITE);

		// ���֡��
		wchar_t str[32] = { 0 };
		wsprintf(str, L"fps: %d", (int)fps);
		outtextxy(0, 0, str);

		// ��ȡ�û������¼�
		msg = getmessage(EM_MOUSE | EM_KEY);

		// ������϶�������ת
		if (msg.lbutton)
		{
			if (old_x != -1)
			{
				pScence->GetObjects()[0].RotateY(-(old_x - msg.x));
				pScence->GetObjects()[0].RotateX(-(msg.y - old_y));
				pScence->GetObjects()[0].UpdateRotatedPoints();
			}

			old_x = msg.x;
			old_y = msg.y;
		}
		else
		{
			old_x = old_y = -1;
		}

		// R ��������
		if (msg.vkcode == 'R')
		{
			pScence->GetObjects()[0].SetAttitude({ 0,0,0 });
			pScence->GetObjects()[0].UpdateRotatedPoints();
			printf("reset\n");
		}

		// ESC �����˳�����
		if (msg.vkcode == VK_ESCAPE)
		{
			break;
		}

		// P �����л�����
		if (msg.vkcode == 'P' && !msg.prevdown)
		{
			if (pScence == &scenceMain)
			{
				pScence = &scence2;
			}
			else
			{
				pScence = &scenceMain;
			}
		}

		// ���֣��ı���� z ��λ��
		if (msg.wheel != 0)
		{
			pScence->MoveCameraZ(msg.wheel / 10);
		}

		// ��������Ϣ������ͼ���Լ���ջ�ͼ������
		flushmessage();
		FlushBatchDraw();
		cleardevice();

		// temp : out put
		Point3D pCenter = scenceMain.GetObjects()[0].GetCenterPoint();
		double pObjMinZ = scenceMain.GetObjects()[0].GetRectangle().min_z;
		Point3D pCamera = scenceMain.GetCameraPosition();
		int nFocalLength = scenceMain.GetCameraFocalLength();
		system("cls");
		printf("�������ĵ� (%.2f, %.2f, %.2f)\n",pCenter.x, pCenter.y, pCenter.z);
		printf("���λ�õ� (%.2f, %.2f, %.2f)\n", pCamera.x, pCamera.y, pCamera.z);
		printf("���ࣺ%d\n", nFocalLength);
		printf("���������ľ��루obj.z - cam.z����%.2f\n", pCenter.z - pCamera.z);
		printf("����� min z ������ľ��룺%.2f\n", pObjMinZ - pCamera.z);
	}

	// ����������ͼ���رջ�ͼ�豸
	EndBatchDraw();
	CloseDrawingDevice();

	return 0;
}
