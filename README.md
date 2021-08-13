# HuiDong3D 渲染库

- [HuiDong3D 渲染库](#huidong3d----)
    + [环境](#environment)
    + [简介](#--)
    + [效果](#--)
    + [功能列表](#----)
    + [已知bug](#--bug)
    + [目标](#--)
    + [示例代码](#----)

### <span id="environment">环境</span>

EasyX 2021-7-30 + VS2019

EasyX 图形库官网： easyx.cn

---

### 简介

由于本人暂时只有初中数学知识……也是自学的 3D 知识，所以暂时做的还不够好。

现在这个渲染库还欠缺很多功能，一起努力学习吧！

加油！

---

### 效果

![人脸模型](https://github.com/zouhuidong/HuiDong3D/blob/master/screenshots/2.png)
![人脸模型](https://github.com/zouhuidong/HuiDong3D/blob/master/screenshots/3.png)
![立方体](https://github.com/zouhuidong/HuiDong3D/blob/master/screenshots/1.png)

---

### 功能列表

- [x] 3D 信息存储
- [x] 3D 旋转运算
- [x] 多边形网格
- [x] 平行投影渲染
- [x] 透视投影渲染（有一点点小bug）
- [x] 视口裁剪（但是目前只是很简单的裁剪，以后更新）
- [x] 创建多个 3D 物体
- [x] 创建多个 3D 场景
- [x] 摄像机自定义调节
- [ ] UV 纹理
- [ ] amp 并行计算
- [ ] 光线追踪

---

### 已知bug

透视渲染时如果物体距离摄像机比较远，会出现扭曲。

---

### 目标

以后还要再继续完善视口裁剪功能，实现 UV 贴图。如果以后有时间的话，可以尝试使用 amp 进行计算。

对了，帧率也要优化一下，现在的帧率有点寒酸……

但是……马上要开学了，不知道以后还有没有时间了，希望我可以继续把这个渲染库完善下去。

---

### 示例代码

使用命名空间 HD3D

```cpp

#include "HuiDong3D.h"
#include <conio.h>

// 获取一段立体柱子的多边形数组
HD3D::Polygon3D* GetPillar();

int main()
{
	// 
	// HuiDong3D Ver 0.1.1(beta) 示例代码
	// 
	// 其中含有少量 easyx 图形库的调用操作
	// easyx 图形库的详细用法见 easyx.cn
	//

	// 创建绘图设备
	HD3D::InitDrawingDevice(640, 480);

	// 设置绘图设备背景颜色（easyx 图形库）
	setbkcolor(LIGHTBLUE);
	cleardevice();

	// 创建 3D 场景
	HD3D::Scence3D scenceMain;

	// 创建一个 3D 物体
	HD3D::Object3D obj;

	// 在物体中加入一个 3D 柱子
	// GetPillar() 函数用来获取柱子的 3D 多边形网格
	obj.AddPolygons(GetPillar(), 6);

	// 将物体加入场景
	scenceMain.AddObject(obj);

	// 物体旋转
	scenceMain.GetObjects()[0].RotateX(-60);
	scenceMain.GetObjects()[0].RotateY(-40);

	// 旋转后要计算新的物体坐标
	scenceMain.GetObjects()[0].UpdateRotatedPoints();

	// 移动相机
	scenceMain.SetCameraPosition({ 0,0,-200 });

	// 渲染场景，并获取渲染耗时
	// 前两个参数指定输出图像的位置，第三个参数指定图像的缩放倍率，
	// 第四个参数指定多边形网格的轮廓线颜色（为 -1 时表示不输出轮廓线）
	double fps = 1.0 / scenceMain.Render(-300, -100, { 0.6,0.6 }, WHITE);

	// 输出渲染耗时（easyx 图形库）
	wchar_t str[32] = { 0 };
	wsprintf(str, L"fps: %d", (int)fps);
	outtextxy(0, 0, str);

	// 按任意键退出
	_getch();

	// 关闭绘图设备
	HD3D::CloseDrawingDevice();
	return 0;
}

// 获取一段 3D 柱子的多边形集合
HD3D::Polygon3D* GetPillar()
{
	HD3D::Polygon3D* pPolygons = new HD3D::Polygon3D[6];
	HD3D::Point3D pPoints[8] = {
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

```

更为详尽、完整的例子请见 main.cpp，同时，HuiDong3D.h 中也有各个函数的详细注释。

