/**
 * @file	HuiDong3D.h
 * @brief	3D 运算库的头文件
 *
 * @author	huidong
 * @mail	huidong_mail@163.com
 * @date	2021.8.2
 *
 */

#pragma once

#include <math.h>
#include <time.h>

 // 图形库
#include <easyx.h>

#include <stdio.h>

// sort
#include <algorithm>
using namespace std;

// 多边形最大边数
#define POLYGON_MAX_SIDES 16

// 代码耗时计算（仅debug状态下有效）
#ifdef _DEBUG

double dfq;
LARGE_INTEGER fq, t_begin, t_end;
#define TIMEC_INIT    {QueryPerformanceFrequency(&fq);dfq=1.0/fq.QuadPart;}
#define TIMEC_BEGIN    QueryPerformanceCounter(&t_begin);
#define TIMEC_END    {QueryPerformanceCounter(&t_end);printf("%lf\n",(t_end.QuadPart-t_begin.QuadPart)*dfq);}

#else

#define TIMEC_INIT
#define TIMEC_BEGIN
#define TIMEC_END

#endif // _DEBUG

/** @brief 颜色信息（为负数则表示无颜色） */
typedef int Color;


/** @brief 2D 坐标 */
struct Point2D
{
	double x;
	double y;
};

/** @brief 3D 坐标 */
struct Point3D
{
	double x;
	double y;
	double z;
};

/** @brief 带有颜色的 3D 坐标 */
struct ColorPoint3D
{
	double x;
	double y;
	double z;
	Color color;

	/** @brief 转换为不带颜色的 3D 坐标 */
	operator Point3D() const { return { x,y,z }; }
};

/** @brief 3D 立方体，存储立方体的最大和最小的 x,y,z 值 */
struct Rectangle3D
{
	double min_x, min_y, min_z, max_x, max_y, max_z;
};



/** @brief 3D 多边形（带颜色） */
class Polygon3D
{
public:

	Polygon3D()
	{
		pPoints = new Point3D[POLYGON_MAX_SIDES];
		memset(pPoints, 0, sizeof Point3D * POLYGON_MAX_SIDES);
		nPointsNum = 0;
		color = 0;
	}

	/**
	 * @brief		根据顶点创建多边形
	 * @param[in]	p: 顶点数组
	 * @param[in]	n: 顶点数量
	 * @param[in]	c: 多边形填充颜色
	 * @attention	顶点数量不得超过最大顶点数量
	 * @see			POLYGON_MAX_SIDES
	*/
	Polygon3D(const Point3D* p, int n, Color c)
	{
		Polygon3D();
		memcpy(pPoints, p, sizeof Point3D * n);
		nPointsNum = n;
		color = c;
	}

	~Polygon3D()
	{
		if (pPoints) delete[] pPoints;

	}

	Point3D* pPoints;	/** @brief 多边形顶点 */
	int nPointsNum;			/** @brief 多边形顶点数量 */
	Color color;			/** @brief 多边形填充颜色 */
};
/**
 * @brief		3D 物体的姿态
 * @attention	姿态的旋转角度都是基于物体自身的，并非相对于旋转原点
 * @note		以下角度都以顺时针作为正的旋转方向
 */
struct Attitude3D
{
	double a;	/** @brief 物体自身的 z 轴旋转角 */
	double e;	/** @brief 物体自身的 y 轴旋转角 */
	double r;	/** @brief 物体自身的 x 轴旋转角 */
};

/** @brief 旋转顺序 */
enum RotateOrder { rotate_x, rotate_y, rotate_z };

/** @brief 默认旋转顺序 */
const int m_defaultRotateOrder[3] = { rotate_z,rotate_y,rotate_x };

/** @brief 将不带颜色的 3D 坐标转换为带颜色的 3D 坐标 */
inline ColorPoint3D ToColorPoint3D(Point3D p)
{
	return { p.x,p.y,p.z,(Color)0 };
}

/**
 * @brief		将角度转换为弧度
 * @param[in]	angle: 原角度
 * @return		角度转换后得到的弧度
*/
inline double ConvertToRadian(double angle)
{
	return 3.1415926535 / 180.0 * angle;
}

/**
 * @brief		旋转 2D 坐标
 * @param[in]	x: x 坐标
 * @param[in]	y: y 坐标
 * @param[in]	angle: 顺时针旋转角度，为负数则表示逆时针旋转
 * @return		旋转后的 2D 坐标
*/
inline Point2D Rotate2D(double x, double y, double angle)
{
	if (angle == 0)	return { x,y };
	double t = ConvertToRadian(angle);
	return { x * cos(t) - y * sin(t),x * sin(t) + y * cos(t) };
}

/**
 * @brief		顺时针旋转 2D 坐标
 * @param[in]	p: 要旋转的 2D 坐标
 * @param[in]	angle: 旋转角度
 * @return		旋转后的 2D 坐标
*/
inline Point2D Rotate2D(Point2D p, double angle)
{
	return Rotate2D(p.x, p.y, angle);
}

/**
 * @brief		将 3D 坐标沿 X 轴顺时针旋转
 * @param[in]	p: 要旋转的坐标
 * @param[in]	angle: 旋转角度
 * @param[in]	pOrigin: 旋转原点
 * @return		旋转后的坐标
*/
inline Point3D Rotate3D_X(Point3D p, double angle, Point3D pOrigin = { 0,0,0 })
{
	Point2D p2 = Rotate2D({ p.y - pOrigin.y,p.z - pOrigin.z }, angle);
	return { p.x,p2.x + pOrigin.y,p2.y + pOrigin.z };
}

/**
 * @brief		将 3D 坐标沿 Y 轴顺时针旋转
 * @param[in]	p: 要旋转的坐标
 * @param[in]	angle: 旋转角度
 * @param[in]	pOrigin: 旋转原点
 * @return		旋转后的坐标
*/
inline Point3D Rotate3D_Y(Point3D p, double angle, Point3D pOrigin = { 0,0,0 })
{
	Point2D p2 = Rotate2D({ p.x - pOrigin.x,p.z - pOrigin.z }, angle);
	return { p2.x + pOrigin.x,p.y,p2.y + pOrigin.z };
}

/**
 * @brief		将 3D 坐标沿 Z 轴顺时针旋转
 * @param[in]	p: 要旋转的坐标
 * @param[in]	angle: 旋转角度
 * @param[in]	pOrigin: 旋转原点
 * @return		旋转后的坐标
*/
inline Point3D Rotate3D_Z(Point3D p, double angle, Point3D pOrigin = { 0,0,0 })
{
	Point2D p2 = Rotate2D({ p.x - pOrigin.x,p.y - pOrigin.y }, angle);
	return { p2.x + pOrigin.x,p2.y + pOrigin.y,p.z };
}

/**
 * @brief		旋转 3D 坐标
 * @param[in]	p: 原坐标
 * @param[in]	a: 绕 z 轴的旋转角度
 * @param[in]	e: 绕 y 轴的旋转角度
 * @param[in]	r: 绕 x 轴的旋转角度
 * @param[in]	pOrigin: 旋转原点
 * @param[in]	pOrder: 旋转顺序
 * @return		旋转后的坐标
*/
inline Point3D Rotate3D(Point3D p, double a, double e, double r, Point3D pOrigin = { 0,0,0 }, const int pOrder[3] = m_defaultRotateOrder)
{
	for (int i = 0; i < 3; i++)
	{
		switch (pOrder[i])
		{
		case rotate_x: p = Rotate3D_X(p, r, pOrigin); break;
		case rotate_y: p = Rotate3D_Y(p, e, pOrigin); break;
		case rotate_z: p = Rotate3D_Z(p, a, pOrigin); break;
		}
	}
	return p;
}

/**
 * @brief		初始化绘图设备
 * @param[in]	w: 绘图设备宽
 * @param[in]	w: 绘图设备高
 * @param[in]	flag: 附加属性，默认为 0
*/
inline void InitDrawingDevice(int w, int h, int flag = 0)
{
	// easyx
	initgraph(w, h, flag);
	//setorigin(w / 4, h / 4);
}

/**
 * @brief	关闭绘图设备
 */
inline void CloseDrawingDevice()
{
	// easyx
	closegraph();
}

/**
 * @brief		绘制点
 * @param[in]	x: 绘制位置
 * @param[in]	y: 绘制位置
 * @param[in]	c: 绘制颜色
 */
inline void DrawPixel(int x, int y, Color c)
{
	if (c < 0) return;

	// easyx 绘制
	// 直接操作显存画点
	if (x >= 0 && y >= 0 && x < getwidth() && y < getheight())
		GetImageBuffer()[y * getwidth() + x] = BGR((COLORREF)c);
}

/**
 * @brief		绘制点
 * @param[in]	p: 绘制位置
 * @param[in]	c: 绘制颜色
 */
inline void DrawPixel(Point2D p, Color c)
{
	DrawPixel((int)p.x, (int)p.y, c);
}

/**
 * @brief		绘制填充多边形
 * @param[in]	p: 3D 多边形
 * @param[in]	grid: 网格颜色，为负数表示不绘制网格
 * @attention	只取多边形的 x,y 坐标绘制到屏幕
 */
inline void DrawFillPolygon(Polygon3D& p, Color grid = -1)
{
	if (p.nPointsNum <= 0) return;

	POINT* pPoints = new POINT[p.nPointsNum];
	for (int j = 0; j < p.nPointsNum; j++)
		pPoints[j] = { (int)p.pPoints[j].x,(int)p.pPoints[j].y };

	//// easyx 绘制

	// 有填充颜色，需要输出	
	if (p.color > 0)
	{
		// 单点
		if (p.nPointsNum == 1)
		{
			DrawPixel(pPoints[0].x, pPoints[0].y, p.color);
		}
		else
		{
			// 有特定网格颜色
			if (grid >= 0)
			{
				setlinecolor((COLORREF)grid);
			}
			else
			{
				setlinecolor((COLORREF)p.color);
			}
			setfillcolor((COLORREF)p.color);
			fillpolygon(pPoints, p.nPointsNum);
		}
	}

	// 无填充颜色，不绘制多边形，除非需要绘制网格
	else
	{
		if (grid >= 0)
		{
			setlinecolor((COLORREF)grid);
			polygon(pPoints, p.nPointsNum);
		}
	}

	delete[] pPoints;
}

/** @brief 比较 Polygon3D 的深度 */
inline bool compPolygonDepth(const Polygon3D a, const Polygon3D b)
{
	return a.pPoints[0].z > b.pPoints[0].z;
}

/**
 * @brief	多边形深度的快速排序（请调用另一个重载函数）
 * @note	改自菜鸟教程
*/
inline void SortPolygons(Polygon3D* s, int l, int r)
{
	if (l < r)
	{
		int i = l, j = r;
		Polygon3D x = s[l];
		while (i < j)
		{
			while (i < j && s[j].pPoints[0].z >= x.pPoints[0].z)
				j--;
			if (i < j)
				s[i++] = s[j];

			while (i < j && s[i].pPoints[0].z < x.pPoints[0].z)
				i++;
			if (i < j)
				s[j--] = s[i];
		}
		s[i] = x;
		SortPolygons(s, l, i - 1);
		SortPolygons(s, i + 1, r);
	}
}

/**
 * @brief			多边形深度的快速排序
 * @param[in/out]	s: 多边形数组
 * @param[in]		size: 数组大小
 * @return			不返回值，排序的结果存放在数组中
*/
inline void SortPolygons(Polygon3D *s, int size)
{
	SortPolygons(s, 0, size - 1);
}

/** @brief 3D 物体 */
class Object3D
{
private:

	Polygon3D* pPolygons;			/** @brief 物体的多边形坐标 */
	Polygon3D* pRotatedPolygons;	/** @brief 计算过姿态后物体的多边形坐标 */
	int nPolygonsNum;				/** @brief 物体的多边形数量 */

	Point3D pCenter;		/** @brief 物体中心点 */
	Attitude3D attitude;	/** @brief 物体姿态 */
	int rotate_order[3];	/** @brief 旋转顺序 */

	/**
	 * @brief	更新物体中心点位置
	*/
	void UpdateCenterPoint()
	{
		Rectangle3D r = GetRectangle();

		pCenter = {
			(r.max_x - r.min_x) / 2 + r.min_x,
			(r.max_y - r.min_y) / 2 + r.min_y,
			(r.max_z - r.min_z) / 2 + r.min_z
		};
	}

	/**
	 * @brief		更新旋转点数组长度
	 * @attention	将擦除原数组中的数据
	*/
	void UpdateRotatedPointsArrayLength()
	{
		Polygon3D* newArray = new Polygon3D[nPolygonsNum];
		if (pRotatedPolygons)
			delete[] pRotatedPolygons;
		pRotatedPolygons = newArray;
	}

	/**
	 * @brief		更新元素数组
	 * @attention	物体的元素数量发生更改时，调用此函数
	*/
	void UpdateArray()
	{
		UpdateCenterPoint();
		UpdateRotatedPointsArrayLength();
		UpdateRotatedPoints();
	}

public:

	Object3D()
	{
		pPolygons = NULL;
		pRotatedPolygons = NULL;
		nPolygonsNum = 0;
		attitude = { 0,0,0 };
		pCenter = { 0,0,0 };
		rotate_order[0] = rotate_z;
		rotate_order[1] = rotate_y;
		rotate_order[2] = rotate_x;
	}

	~Object3D()
	{
		if (pPolygons)			delete[] pPolygons;
		if (pRotatedPolygons)	delete[] pRotatedPolygons;
	}

	/** @brief 设置旋转顺序 */
	void SetRotateOrder(int pOrder[3])
	{
		for (int i = 0; i < 3; i++)
			rotate_order[i] = pOrder[i];
	}

	/** @brief 获取旋转顺序 */
	int* GetRotateOrder()
	{
		return rotate_order;
	}

	/** @brief 获取物体中点的数量 */
	int GetPointsNum()
	{
		int num = 0;
		for (int i = 0; i < nPolygonsNum; i++)
		{
			num += pPolygons[i].nPointsNum;
		}
		return num;
	}

	/** @brief 获取物体中多边形的数量 */
	int GetPolygonsNum()
	{
		return nPolygonsNum;
	}

	/**
	 * @brief		获取物体的所有点
	 * @param[in]	rotated: 是否获取旋转后的点
	 * @attention	调用 GetPointsNum 函数来获取点的数量
	*/
	ColorPoint3D* GetPoints(bool rotated = true)
	{
		Polygon3D* polygons = pPolygons;
		if (rotated) polygons = pRotatedPolygons;
		ColorPoint3D* p = new ColorPoint3D[GetPointsNum()];
		for (int i = 0, index = 0; i < nPolygonsNum; i++)
		{
			for (int j = 0; j < polygons[i].nPointsNum; j++)
			{
				p[index] = ToColorPoint3D(polygons[i].pPoints[j]);
				p[index].color = polygons[i].color;
				index++;
			}
		}
		return p;
	}

	/**
	 * @brief		获取物体中的所有多边形
	 * @param[in]	rotated: 是否获取旋转后的多边形
	 * @attention	调用 GetPolygonsNum 函数来获取多边形的数量
	*/
	Polygon3D* GetPolygons(bool rotated = true)
	{
		if (rotated)
		{
			return pRotatedPolygons;
		}
		else
		{
			return pPolygons;
		}
	}

	/** @brief 获取物体中心点坐标 */
	Point3D GetCenterPoint()
	{
		return pCenter;
	}

	/**  @brief	获取物体位置，效果与 GetCenterPoint 函数相同 */
	Point3D GetPosition()
	{
		return GetCenterPoint();
	}

	/** @brief 移动物体到某位置，并立即计算新坐标 */
	void MoveTo(Point3D pNew)
	{
		double offset_x = pNew.x - pCenter.x;
		double offset_y = pNew.y - pCenter.y;
		double offset_z = pNew.z - pCenter.z;
		for (int i = 0; i < nPolygonsNum; i++)
		{
			for (int j = 0; j < pPolygons[i].nPointsNum; j++)
			{
				pPolygons[i].pPoints[j].x += offset_x;
				pPolygons[i].pPoints[j].y += offset_y;
				pPolygons[i].pPoints[j].z += offset_z;

				pRotatedPolygons[i].pPoints[j].x += offset_x;
				pRotatedPolygons[i].pPoints[j].y += offset_y;
				pRotatedPolygons[i].pPoints[j].z += offset_z;
			}
		}

		pCenter = pNew;
	}

	/** @brief 将物体沿 x 轴方向移动，并立即计算新坐标 */
	void MoveX(double n)
	{
		for (int i = 0; i < nPolygonsNum; i++)
		{
			for (int j = 0; j < pPolygons[i].nPointsNum; j++)
			{
				pPolygons[i].pPoints[j].x += n;
				pRotatedPolygons[i].pPoints[j].x += n;
			}
		}

		pCenter.x += n;
	}

	/** @brief 将物体沿 y 轴方向移动，并立即计算新坐标 */
	void MoveY(double n)
	{
		for (int i = 0; i < nPolygonsNum; i++)
		{
			for (int j = 0; j < pPolygons[i].nPointsNum; j++)
			{
				pPolygons[i].pPoints[j].y += n;
				pRotatedPolygons[i].pPoints[j].y += n;
			}
		}

		pCenter.y += n;
	}

	/** @brief 将物体沿 z 轴方向移动，并立即计算新坐标 */
	void MoveZ(double n)
	{
		for (int i = 0; i < nPolygonsNum; i++)
		{
			for (int j = 0; j < pPolygons[i].nPointsNum; j++)
			{
				pPolygons[i].pPoints[j].z += n;
				pRotatedPolygons[i].pPoints[j].z += n;
			}
		}

		pCenter.z += n;
	}

	/** @brief 设置物体姿态 */
	void SetAttitude(Attitude3D ati)
	{
		attitude = ati;
	}

	/** @brief 获取物体姿态 */
	Attitude3D GetAttitude()
	{
		return attitude;
	}

	/** @brief 绕 x 轴顺时针调整物体姿态 */
	void RotateX(double angle)
	{
		attitude.r += angle;
	}

	/** @brief 绕 y 轴顺时针调整物体姿态 */
	void RotateY(double angle)
	{
		attitude.e += angle;
	}

	/** @brief 绕 z 轴顺时针调整物体姿态 */
	void RotateZ(double angle)
	{
		attitude.a += angle;
	}

	/** @brief 获取物体的三维矩形边框 */
	Rectangle3D GetRectangle()
	{
		Rectangle3D r;
		for (int i = 0; i < nPolygonsNum; i++)
		{
			for (int j = 0; j < pPolygons[i].nPointsNum; j++)
			{
				// init
				if (i + j == 0)
				{
					r.min_x = r.max_x = pPolygons[i].pPoints[j].x;
					r.min_y = r.max_y = pPolygons[i].pPoints[j].y;
					r.min_z = r.max_z = pPolygons[i].pPoints[j].z;
					continue;
				}

				if (pPolygons[i].pPoints[j].x < r.min_x)
					r.min_x = pPolygons[i].pPoints[j].x;
				else if (pPolygons[i].pPoints[j].x > r.max_x)
					r.max_x = pPolygons[i].pPoints[j].x;

				if (pPolygons[i].pPoints[j].y < r.min_y)
					r.min_y = pPolygons[i].pPoints[j].y;
				else if (pPolygons[i].pPoints[j].y > r.max_y)
					r.max_y = pPolygons[i].pPoints[j].y;

				if (pPolygons[i].pPoints[j].z < r.min_z)
					r.min_z = pPolygons[i].pPoints[j].z;
				else if (pPolygons[i].pPoints[j].z > r.max_z)
					r.max_z = pPolygons[i].pPoints[j].z;
			}
		}
		return r;
	}

	/**
	 * @brief		更新旋转点数组，计算调整姿态后的新坐标
	 * @attention	设置或调整物体姿态后不会立即计算新的物体坐标，需要手动调用
	 *				此函数进行计算。
	*/
	void UpdateRotatedPoints()
	{
		for (int i = 0; i < nPolygonsNum; i++)
		{
			for (int j = 0; j < pPolygons[i].nPointsNum; j++)
			{
				pRotatedPolygons[i].pPoints[j] = Rotate3D(pPolygons[i].pPoints[j], attitude.a, attitude.e, attitude.r, pCenter, rotate_order);
			}
			pRotatedPolygons[i].nPointsNum = pPolygons[i].nPointsNum;
			pRotatedPolygons[i].color = pPolygons[i].color;
		}
	}

	/**
	 * @brief		在物体中添加点
	 * @attention	此函数中以单个点为单位添加，所以每个点都独立作为一个多边形插入数组
	 * @param[in]	pNew: 要添加的点的数组
	 * @param[in]	num: 要添加的点的数量
	 * @return		返回新添加的（多个）点在数组中的第一个索引，若插入数据失败返回 -1
	*/
	int AddPoints(ColorPoint3D* pNew, int num)
	{
		if (num <= 0 || !pNew)	return -1;

		Polygon3D* newArray = new Polygon3D[nPolygonsNum + num];
		Polygon3D* p = pPolygons;
		for (int i = 0, j = 0; i < nPolygonsNum; i++, j++)
		{
			memcpy(newArray[i].pPoints, p[j].pPoints, sizeof Point3D * POLYGON_MAX_SIDES);
			newArray[i].nPointsNum = p[j].nPointsNum;
			newArray[i].color = p[j].color;
		}
		for (int i = nPolygonsNum, j = 0; j < num; i++, j++)
		{
			newArray[i].pPoints[0] = pNew[j];
			newArray[i].nPointsNum = 1;
			newArray[i].color = pNew[j].color;
		}
		if (pPolygons) delete[] pPolygons;
		pPolygons = newArray;
		nPolygonsNum += num;

		UpdateArray();

		return nPolygonsNum - num;
	}

	/**
	 * @brief		在物体中添加多边形
	 * @param[in]	pNew: 要添加的多边形的数组
	 * @param[in]	num: 要添加的多边形的数量
	 * @return		返回新添加的（多个）多边形在数组中的第一个索引，
	 *				若插入数据失败返回 -1
	 * @attention	由于添加过程中复制了 pNew 中的 pPoints 的地址，
	 *				所以不要删除 pNew 对应的 pPoints
	*/
	int AddPolygons(Polygon3D* pNew, int num)
	{
		if (num <= 0 || !pNew)	return -1;

		Polygon3D* newArray = new Polygon3D[nPolygonsNum + num];
		Polygon3D* p = pPolygons;
		for (int i = 0, j = 0; i < nPolygonsNum + num; i++, j++)
		{
			if (i == nPolygonsNum)
			{
				p = pNew;
				j = 0;
			}
			memcpy(newArray[i].pPoints, p[j].pPoints, sizeof Point3D * POLYGON_MAX_SIDES);
			newArray[i].nPointsNum = p[j].nPointsNum;
			newArray[i].color = p[j].color;
		}
		if (pPolygons) delete[] pPolygons;
		pPolygons = newArray;
		nPolygonsNum += num;

		UpdateArray();

		return nPolygonsNum - num;
	}

	/**
	 * @brief		删除物体中的某个多边形
	 * @param[in]	p: 要删除的点的索引
	*/
	void DeletePolygon(int index)
	{
		Polygon3D* newArray = new Polygon3D[nPolygonsNum - 1];
		for (int i = 0, j = 0; j < nPolygonsNum; i++, j++)
		{
			if (j != index)
			{
				newArray[i] = pPolygons[j];
			}
			else
			{
				i--;
			}
		}

		if (pPolygons)
			delete[] pPolygons;
		pPolygons = newArray;
		nPolygonsNum--;

		UpdateArray();
	}

};

/** @brief 3D 场景 */
class Scence3D
{

private:
	Object3D* pObjects;	/** @brief 场景内的物体集合 */
	int nObjectsNum;	/** @brief 场景内的物体数量 */

	Point3D pCamera;			/** @brief 相机坐标 */
	Attitude3D attitudeCamera;	/** @brief 相机姿态 */
	int nViewportWidth;			/** @brief 相机视口宽度 */
	int nViewportHeight;		/** @brief 相机视口高度 */
	int nFocalLength;			/** @brief 相机焦距 */


public:

	Scence3D()
	{
		pObjects = NULL;
		nObjectsNum = 0;

		pCamera = { 0,0,-100 };
		attitudeCamera = { 0,0,0 };

		nViewportWidth = 500;
		nViewportHeight = 500;
		nFocalLength = 200;
	}

	~Scence3D()
	{
		if (pObjects) delete[] pObjects;
	}

	/** @brief 设置相机位置 */
	void SetCameraPosition(Point3D p)
	{
		pCamera = p;
	}

	/** @brief 获取相机位置 */
	Point3D GetCameraPosition()
	{
		return pCamera;
	}

	/** @brief 移动相机，效果与 SetCameraPosition 相同 */
	void MoveCameraTo(Point3D p)
	{
		SetCameraPosition(p);
	}

	/** @brief 在 x 轴方向移动相机 */
	void MoveCameraX(double n)
	{
		pCamera.x += n;
	}

	/** @brief 在 y 轴方向移动相机 */
	void MoveCameraY(double n)
	{
		pCamera.y += n;
	}

	/** @brief 在 z 轴方向移动相机 */
	void MoveCameraZ(double n)
	{
		pCamera.z += n;
	}

	/** @brief 设置相机姿态 */
	void SetCameraAttitude(Attitude3D ati)
	{
		attitudeCamera = ati;
	}

	/** @brief 获取相机姿态 */
	Attitude3D GetCameraAttitude()
	{
		return attitudeCamera;
	}

	/** @brief 将相机绕 x 轴旋转 */
	void RotateCameraX(double angle)
	{
		attitudeCamera.r += angle;
	}

	/** @brief 将相机绕 y 轴旋转 */
	void RotateCameraY(double angle)
	{
		attitudeCamera.e += angle;
	}

	/** @brief 将相机绕 z 轴旋转 */
	void RotateCameraZ(double angle)
	{
		attitudeCamera.a += angle;
	}

	/** @brief 设置相机视口大小 */
	void SetCameraViewportSize(int w, int h)
	{
		nViewportWidth = w;
		nViewportHeight = h;
	}

	/**
	 * @brief		获取相机视口大小
	 * @param[out]	w: 视口宽度
	 * @param[out]	h: 视口高度
	*/
	void GetCameraViewportSize(int* w, int* h)
	{
		*w = nViewportWidth;
		*h = nViewportHeight;
	}

	/** @brief 设置相机焦距 */
	void SetCameraFocalLength(int f)
	{
		nFocalLength = f;
	}

	/** @brief 获取相机焦距 */
	int GetCameraFocalLength()
	{
		return nFocalLength;
	}

	/**
	 * @brief		在场景中添加物体
	 * @param[in]	obj: 要添加的物体
	 * @return		返回添加的物体在数组中的索引
	*/
	int AddObject(Object3D& obj)
	{
		Object3D* newObjects = new Object3D[nObjectsNum + 1];
		Object3D* p = pObjects;
		for (int i = 0; i < nObjectsNum + 1; i++)
		{
			if (i == nObjectsNum)
			{
				p = &obj;
			}
			newObjects[i].AddPolygons(p[i].GetPolygons(), p[i].GetPolygonsNum());
			newObjects[i].SetAttitude(p[i].GetAttitude());
			newObjects[i].SetRotateOrder(p[i].GetRotateOrder());
		}
		if (pObjects) delete[] pObjects;
		pObjects = newObjects;
		nObjectsNum++;
		return nObjectsNum - 1;
	}

	/**
	 * @brief		删除场景中的物体
	 * @param[in]	pObj: 要删除的物体的索引
	*/
	void DeleteObject(int index)
	{
		Object3D* newObjects = new Object3D[nObjectsNum - 1];
		for (int i = 0, j = 0; j < nObjectsNum; i++, j++)
		{
			if (j != index)
			{
				newObjects[i] = pObjects[j];
			}
			else
			{
				i--;
			}
		}
		if (pObjects)
			delete[] pObjects;
		pObjects = newObjects;
		nObjectsNum--;
	}

	/** @brief 获取场景中物体的集合 */
	Object3D* GetObjects()
	{
		return pObjects;
	}

	/** @brief 获取场景中物体的数量 */
	int GetObjectsNum()
	{
		return nObjectsNum;
	}

	/** @brief 获取场景中所有的物体的多边形的数量总和 */
	int GetPolygonsNum()
	{
		int index = 0;
		for (int i = 0; i < nObjectsNum; i++)
			index += pObjects[i].GetPolygonsNum();
		return index;
	}

	/** @brief 获取场景中所有物体的多边形的集合 */
	Polygon3D* GetPolygons()
	{
		Polygon3D* p = new Polygon3D[GetPolygonsNum()];
		for (int i = 0, index = 0; i < nObjectsNum; i++)
		{
			Polygon3D* pp = pObjects[i].GetPolygons();
			for (int j = 0; j < pObjects[i].GetPolygonsNum(); j++)
			{
				p[index] = pp[j];
				index++;
			}
		}
		return p;
	}

	/**
	 * @brief		绘制场景到屏幕
	 * @param[in]	offset_x: 输出到屏幕的图像的 x 坐标偏移
	 * @param[in]	offset_y: 输出到屏幕的图像的 y 坐标偏移
	 * @return		返回绘制耗时（单位：秒）
	*/
	double Draw(int offset_x = 0, int offset_y = 0)
	{

		/*
		 *	相机视口
		 *
		 *		正视图
		 *
		 *            ↑ y ( viewport coordinate system )
		 *            |
		 *			_ | ____________________
		 *			↑ +----------------------+
		 *			| |   |              |   |
		 *			| |   |              |   |
		 *	viewport| |   |______________|   |
		 *	height  | |  /       ↑        \  |
		 *			| | /        | focal   \ |
		 *			| |/         ↓ length   \|
		 *			↓[O]---------------------+ ———————> x ( viewport coordinate system )
		 *		    / |←—— viewport_width ——→|
		 *			|
		 *			|
		 *			|_--→ viewport origin
		 *
		 *
		 *
		 *		侧视图
		 *
		 *							    ↑ y ( viewport coordinate system )
		 *            __________________|__
		 *           /                  | /|
		 *			|←-- focal length -→|/ |   __  x ( viewport coordinate system )
		 *			+-------------------/  |    /\
		 *			|   |               |  |   /
		 *			|   |               |  | Oo
		 *			|   |               | }-[==] ----→ camera
		 *			|   |               |  |/
		 *			|___|_______________|  /
		 *	  <—————+------------------[O]-+
		 *   z ( viewport				|
		 *		 coordinate system )	|_----→ viewport origin
		 *
		 */

		TIMEC_INIT;

		int t = clock();

		printf("预初始化 ");
		TIMEC_BEGIN;

		int nAllPolygonsNum = GetPolygonsNum();
		Polygon3D* pAllPolygons = GetPolygons();					// 所有多边形
		Polygon3D* pFilterWide = new Polygon3D[nAllPolygonsNum];	// 泛筛选结果以及旋转后结果
		Polygon3D* pFilterNarrow = new Polygon3D[nAllPolygonsNum];	// 狭筛选结果
		Polygon3D* pFilterDepth = new Polygon3D[nAllPolygonsNum];	// 深度筛选结果
		int nFilterWideNum = 0, nFilterNarrowNum = 0, nFilterDepthNum = 0;

		int** pDepthMap = new int* [nViewportWidth];	// 在相机渲染范围内的每个点的深度信息
		for (int i = 0; i < nViewportWidth; i++)
		{
			// 初始化深度信息数组的值为：相机的深度加两倍焦距，是一个不可能出现在渲染范围内的值
			pDepthMap[i] = new int[nViewportHeight];
			for (int j = 0; j < nViewportHeight; j++)
			{
				pDepthMap[i][j] = (int)pCamera.z + 2 * nFocalLength;
			}
		}

		// 选取相机视口的较长边 (l)，求较长边和焦距组成的平面的对角线长度，即为 h
		int l = nViewportWidth > nViewportHeight ? nViewportWidth : nViewportHeight;
		int h = (int)sqrt(nFocalLength * nFocalLength + l * l);

		int lx = nViewportWidth / 2, ly = nViewportHeight / 2;					// limit_x and limit_y
		Point3D pOriginViewport = { pCamera.x - lx,pCamera.y - ly,pCamera.z };	// 相机视口的原点

		TIMEC_END;
		printf("泛筛选 ");
		TIMEC_BEGIN;

		//// 开始筛选

		// 泛筛选
		for (int i = 0; i < nAllPolygonsNum; i++)
		{
			for (int j = 0; j < pAllPolygons[i].nPointsNum; j++)
			{
				// 若该多边形的其中任意一个顶点在以相机为中心的、边长为 h 的立方体内，才有可能被渲染
				if (pAllPolygons[i].pPoints[j].x >= pCamera.x - h && pAllPolygons[i].pPoints[j].x <= pCamera.x + h &&
					pAllPolygons[i].pPoints[j].y >= pCamera.y - h && pAllPolygons[i].pPoints[j].y <= pCamera.y + h &&
					pAllPolygons[i].pPoints[j].z >= pCamera.z - h && pAllPolygons[i].pPoints[j].z <= pCamera.z + h)
				{
					pFilterWide[nFilterWideNum] = pAllPolygons[i];
					nFilterWideNum++;
					break;
				}
			}
		}

		TIMEC_END;
		printf("旋转 ");
		TIMEC_BEGIN;

		// 对泛筛选出的多边形绕相机旋转，获取相机姿态下，这些多边形的坐标
		for (int i = 0; i < nFilterWideNum; i++)
		{
			for (int j = 0; j < pFilterWide[i].nPointsNum; j++)
			{
				pFilterWide[i].pPoints[j] = Rotate3D(pFilterWide[i].pPoints[j], -attitudeCamera.a, -attitudeCamera.e, -attitudeCamera.r, pCamera);
			}
		}

		TIMEC_END;
		printf("转换坐标系 ");
		TIMEC_BEGIN;

		// 旋转后将坐标转到 相机视口 的坐标系
		for (int i = 0; i < nFilterWideNum; i++)
		{
			for (int j = 0; j < pFilterWide[i].nPointsNum; j++)
			{
				pFilterWide[i].pPoints[j].x -= pOriginViewport.x;
				pFilterWide[i].pPoints[j].y -= pOriginViewport.y;
				pFilterWide[i].pPoints[j].z -= pOriginViewport.z;
			}
		}

		TIMEC_END;
		printf("狭筛选 ");
		TIMEC_BEGIN;

		// 狭筛选
		for (int i = 0; i < nFilterWideNum; i++)
		{
			for (int j = 0; j < pFilterWide[i].nPointsNum; j++)
			{
				// 若该多边形的任意一个顶点在相机所面对的视口立方体中（该立方体的宽高为视口宽高，深度为焦距），则判定此多边形在相机的渲染范围内
				if (pFilterWide[i].pPoints[j].x >= 0 && pFilterWide[i].pPoints[j].x <= nViewportWidth &&
					pFilterWide[i].pPoints[j].y >= 0 && pFilterWide[i].pPoints[j].y <= nViewportHeight &&
					pFilterWide[i].pPoints[j].z >= 0 && pFilterWide[i].pPoints[j].z <= nFocalLength)
				{
					pFilterNarrow[nFilterNarrowNum] = pFilterWide[i];
					nFilterNarrowNum++;
					break;
				}
			}
		}

		TIMEC_END;
		printf("记录深度信息 ");
		TIMEC_BEGIN;

		// 深度信息记录
		for (int i = 0; i < nFilterNarrowNum; i++)
		{
			for (int j = 0; j < pFilterNarrow[i].nPointsNum; j++)
			{
				// 遍历狭筛选后的所有点，得出在视口平面上的每个点的最浅深度
				if (pFilterNarrow[i].pPoints[j].z < pDepthMap[(int)pFilterNarrow[i].pPoints[j].x][(int)pFilterNarrow[i].pPoints[j].y])
				{
					pDepthMap[(int)pFilterNarrow[i].pPoints[j].x][(int)pFilterNarrow[i].pPoints[j].y] = (int)pFilterNarrow[i].pPoints[j].z;
				}
			}
		}

		TIMEC_END;
		printf("深度筛选 ");
		TIMEC_BEGIN;

		// 深度筛选：筛除三个顶点的 z 坐标都不在顶层的多边形，因为他们的渲染是无效的
		for (int i = 0; i < nFilterNarrowNum; i++)
		{
			for (int j = 0; j < pFilterNarrow[i].nPointsNum; j++)
			{
				// 该多边形的任一顶点在 z 轴的顶层
				if (pFilterNarrow[i].pPoints[j].z <= pDepthMap[(int)pFilterNarrow[i].pPoints[j].x][(int)pFilterNarrow[i].pPoints[j].y])
				{
					pFilterDepth[nFilterDepthNum] = pFilterNarrow[i];
					nFilterDepthNum++;
					break;
				}
			}
		}

		TIMEC_END;
		printf("排序 ");
		TIMEC_BEGIN;

		// 完成所有筛选，将筛选结果按 z 轴层次排序以决定绘制顺序
		SortPolygons(pFilterDepth, nFilterDepthNum);

		TIMEC_END;
		printf("绘制 ");
		TIMEC_BEGIN;

		// 绘制
		for (int i = nFilterDepthNum - 1; i >= 0; i--)
		{
			DrawFillPolygon(pFilterDepth[i]);
		}

		TIMEC_END;

		delete[] pAllPolygons;
		delete[] pFilterWide;
		delete[] pFilterNarrow;
		delete[] pFilterDepth;
		for(int i=0;i< nViewportWidth;i++)
			delete[] pDepthMap[i];
		delete[] pDepthMap;

		return (double)(clock() - t) / CLOCKS_PER_SEC;
	}

};
