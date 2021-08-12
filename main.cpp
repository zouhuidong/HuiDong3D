#include <stdio.h>
#include "HuiDong3D.h"
using namespace HD3D;

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

// 获取一段 3D 柱子的多边形集合
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

// 获取 3D 柱子物体
Object3D* GetPillarObject()
{
	Object3D* objPillar = new Object3D;
	objPillar->AddPolygons(GetPillar(), 6);

	// 文字
	int nTextPointsNum = 0;
	ColorPoint3D* pTextPoints = NULL;

	/*pTextPoints = GetTextPoint3D(L"文字添加测试", 10, 230, -10, &nTextPointsNum);
	objPillar.AddPoints(pTextPoints, nTextPointsNum);
	delete[] pTextPoints;

	pTextPoints = GetTextPoint3D(L"第二行文字", 40, 200, -10, &nTextPointsNum);
	objPillar.AddPoints(pTextPoints, nTextPointsNum);
	delete[] pTextPoints;*/
	
	pTextPoints = GetTextPoint3D(GetEasyXVer(), 10, 230, -1, &nTextPointsNum);
	objPillar->AddPoints(pTextPoints, nTextPointsNum);
	delete[] pTextPoints;

	return objPillar;
}

// 获取 3D 模型物体
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
	// 初始化绘图设备
	InitDrawingDevice(640, 480, 1);

	// 设置绘图设备背景色
	setbkcolor(RGB(130, 190, 230));
	cleardevice();

	// 3D 场景
	Scence3D scenceMain;
	Scence3D scence2;

	// 加入物体
	scenceMain.AddObject(*GetPillarObject());
	//scenceMain.AddObject(*GetModelObject());

	// 移动物体
	scenceMain.GetObjects()[0].MoveTo({ 0,0,100});

	// 设置相机焦距
	//scenceMain.SetCameraFocalLength(1000);

	// 启用 / 禁用 透视投影
	scenceMain.EnablePerspectiveProjection(true);

	// 第二个场景
	scence2.AddObject(*GetModelObject());

	// 开始批量绘图
	BeginBatchDraw();

	// 获取鼠标消息
	ExMessage msg;
	int old_x = -1, old_y = -1;

	// 当前活动场景的指针
	Scence3D* pScence = &scenceMain;

	// 消息主循环
	while (true)
	{
		// 调用场景渲染器，并获取渲染时间
		double fps = 1.0 / pScence->Render(-300, -200, { 0.6,0.6 }, WHITE);

		// 输出帧率
		wchar_t str[32] = { 0 };
		wsprintf(str, L"fps: %d", (int)fps);
		outtextxy(0, 0, str);

		// 获取用户操作事件
		msg = getmessage(EM_MOUSE | EM_KEY);

		// 左键：拖动物体旋转
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

		// R 键：重置
		if (msg.vkcode == 'R')
		{
			pScence->GetObjects()[0].SetAttitude({ 0,0,0 });
			pScence->GetObjects()[0].UpdateRotatedPoints();
			printf("reset\n");
		}

		// ESC 键：退出程序
		if (msg.vkcode == VK_ESCAPE)
		{
			break;
		}

		// P 键：切换场景
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

		// 滚轮：改变相机 z 轴位置
		if (msg.wheel != 0)
		{
			pScence->MoveCameraZ(msg.wheel / 10);
		}

		// 清空鼠标消息，绘制图像以及清空绘图缓冲区
		flushmessage();
		FlushBatchDraw();
		cleardevice();

		// temp : out put
		Point3D pCenter = scenceMain.GetObjects()[0].GetCenterPoint();
		double pObjMinZ = scenceMain.GetObjects()[0].GetRectangle().min_z;
		Point3D pCamera = scenceMain.GetCameraPosition();
		int nFocalLength = scenceMain.GetCameraFocalLength();
		system("cls");
		printf("物体中心点 (%.2f, %.2f, %.2f)\n",pCenter.x, pCenter.y, pCenter.z);
		printf("相机位置点 (%.2f, %.2f, %.2f)\n", pCamera.x, pCamera.y, pCamera.z);
		printf("焦距：%d\n", nFocalLength);
		printf("物体和相机的距离（obj.z - cam.z）：%.2f\n", pCenter.z - pCamera.z);
		printf("物体的 min z 和相机的距离：%.2f\n", pObjMinZ - pCamera.z);
	}

	// 结束批量绘图，关闭绘图设备
	EndBatchDraw();
	CloseDrawingDevice();

	return 0;
}
