/**
 * @file	HuiDong3D.h
 * @brief	3D ������ͷ�ļ�
 *
 * @author	huidong
 * @mail	huidong_mail@163.com
 * @date	2021.8.2
 *
 */

#pragma once

#include <math.h>
#include <time.h>

 // ͼ�ο�
#include <easyx.h>

#include <stdio.h>

// sort
#include <algorithm>
using namespace std;

// �����������
#define POLYGON_MAX_SIDES 16

// �����ʱ���㣨��debug״̬����Ч��
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

/** @brief ��ɫ��Ϣ��Ϊ�������ʾ����ɫ�� */
typedef int Color;


/** @brief 2D ���� */
struct Point2D
{
	double x;
	double y;
};

/** @brief 3D ���� */
struct Point3D
{
	double x;
	double y;
	double z;
};

/** @brief ������ɫ�� 3D ���� */
struct ColorPoint3D
{
	double x;
	double y;
	double z;
	Color color;

	/** @brief ת��Ϊ������ɫ�� 3D ���� */
	operator Point3D() const { return { x,y,z }; }
};

/** @brief 3D �����壬�洢�������������С�� x,y,z ֵ */
struct Rectangle3D
{
	double min_x, min_y, min_z, max_x, max_y, max_z;
};



/** @brief 3D ����Σ�����ɫ�� */
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
	 * @brief		���ݶ��㴴�������
	 * @param[in]	p: ��������
	 * @param[in]	n: ��������
	 * @param[in]	c: ����������ɫ
	 * @attention	�����������ó�����󶥵�����
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

	Point3D* pPoints;	/** @brief ����ζ��� */
	int nPointsNum;			/** @brief ����ζ������� */
	Color color;			/** @brief ����������ɫ */
};
/**
 * @brief		3D �������̬
 * @attention	��̬����ת�Ƕȶ��ǻ�����������ģ������������תԭ��
 * @note		���½Ƕȶ���˳ʱ����Ϊ������ת����
 */
struct Attitude3D
{
	double a;	/** @brief ��������� z ����ת�� */
	double e;	/** @brief ��������� y ����ת�� */
	double r;	/** @brief ��������� x ����ת�� */
};

/** @brief ��ת˳�� */
enum RotateOrder { rotate_x, rotate_y, rotate_z };

/** @brief Ĭ����ת˳�� */
const int m_defaultRotateOrder[3] = { rotate_z,rotate_y,rotate_x };

/** @brief ��������ɫ�� 3D ����ת��Ϊ����ɫ�� 3D ���� */
inline ColorPoint3D ToColorPoint3D(Point3D p)
{
	return { p.x,p.y,p.z,(Color)0 };
}

/**
 * @brief		���Ƕ�ת��Ϊ����
 * @param[in]	angle: ԭ�Ƕ�
 * @return		�Ƕ�ת����õ��Ļ���
*/
inline double ConvertToRadian(double angle)
{
	return 3.1415926535 / 180.0 * angle;
}

/**
 * @brief		��ת 2D ����
 * @param[in]	x: x ����
 * @param[in]	y: y ����
 * @param[in]	angle: ˳ʱ����ת�Ƕȣ�Ϊ�������ʾ��ʱ����ת
 * @return		��ת��� 2D ����
*/
inline Point2D Rotate2D(double x, double y, double angle)
{
	if (angle == 0)	return { x,y };
	double t = ConvertToRadian(angle);
	return { x * cos(t) - y * sin(t),x * sin(t) + y * cos(t) };
}

/**
 * @brief		˳ʱ����ת 2D ����
 * @param[in]	p: Ҫ��ת�� 2D ����
 * @param[in]	angle: ��ת�Ƕ�
 * @return		��ת��� 2D ����
*/
inline Point2D Rotate2D(Point2D p, double angle)
{
	return Rotate2D(p.x, p.y, angle);
}

/**
 * @brief		�� 3D ������ X ��˳ʱ����ת
 * @param[in]	p: Ҫ��ת������
 * @param[in]	angle: ��ת�Ƕ�
 * @param[in]	pOrigin: ��תԭ��
 * @return		��ת�������
*/
inline Point3D Rotate3D_X(Point3D p, double angle, Point3D pOrigin = { 0,0,0 })
{
	Point2D p2 = Rotate2D({ p.y - pOrigin.y,p.z - pOrigin.z }, angle);
	return { p.x,p2.x + pOrigin.y,p2.y + pOrigin.z };
}

/**
 * @brief		�� 3D ������ Y ��˳ʱ����ת
 * @param[in]	p: Ҫ��ת������
 * @param[in]	angle: ��ת�Ƕ�
 * @param[in]	pOrigin: ��תԭ��
 * @return		��ת�������
*/
inline Point3D Rotate3D_Y(Point3D p, double angle, Point3D pOrigin = { 0,0,0 })
{
	Point2D p2 = Rotate2D({ p.x - pOrigin.x,p.z - pOrigin.z }, angle);
	return { p2.x + pOrigin.x,p.y,p2.y + pOrigin.z };
}

/**
 * @brief		�� 3D ������ Z ��˳ʱ����ת
 * @param[in]	p: Ҫ��ת������
 * @param[in]	angle: ��ת�Ƕ�
 * @param[in]	pOrigin: ��תԭ��
 * @return		��ת�������
*/
inline Point3D Rotate3D_Z(Point3D p, double angle, Point3D pOrigin = { 0,0,0 })
{
	Point2D p2 = Rotate2D({ p.x - pOrigin.x,p.y - pOrigin.y }, angle);
	return { p2.x + pOrigin.x,p2.y + pOrigin.y,p.z };
}

/**
 * @brief		��ת 3D ����
 * @param[in]	p: ԭ����
 * @param[in]	a: �� z �����ת�Ƕ�
 * @param[in]	e: �� y �����ת�Ƕ�
 * @param[in]	r: �� x �����ת�Ƕ�
 * @param[in]	pOrigin: ��תԭ��
 * @param[in]	pOrder: ��ת˳��
 * @return		��ת�������
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
 * @brief		��ʼ����ͼ�豸
 * @param[in]	w: ��ͼ�豸��
 * @param[in]	w: ��ͼ�豸��
 * @param[in]	flag: �������ԣ�Ĭ��Ϊ 0
*/
inline void InitDrawingDevice(int w, int h, int flag = 0)
{
	// easyx
	initgraph(w, h, flag);
	//setorigin(w / 4, h / 4);
}

/**
 * @brief	�رջ�ͼ�豸
 */
inline void CloseDrawingDevice()
{
	// easyx
	closegraph();
}

/**
 * @brief		���Ƶ�
 * @param[in]	x: ����λ��
 * @param[in]	y: ����λ��
 * @param[in]	c: ������ɫ
 */
inline void DrawPixel(int x, int y, Color c)
{
	if (c < 0) return;

	// easyx ����
	// ֱ�Ӳ����Դ滭��
	if (x >= 0 && y >= 0 && x < getwidth() && y < getheight())
		GetImageBuffer()[y * getwidth() + x] = BGR((COLORREF)c);
}

/**
 * @brief		���Ƶ�
 * @param[in]	p: ����λ��
 * @param[in]	c: ������ɫ
 */
inline void DrawPixel(Point2D p, Color c)
{
	DrawPixel((int)p.x, (int)p.y, c);
}

/**
 * @brief		�����������
 * @param[in]	p: 3D �����
 * @param[in]	grid: ������ɫ��Ϊ������ʾ����������
 * @attention	ֻȡ����ε� x,y ������Ƶ���Ļ
 */
inline void DrawFillPolygon(Polygon3D& p, Color grid = -1)
{
	if (p.nPointsNum <= 0) return;

	POINT* pPoints = new POINT[p.nPointsNum];
	for (int j = 0; j < p.nPointsNum; j++)
		pPoints[j] = { (int)p.pPoints[j].x,(int)p.pPoints[j].y };

	//// easyx ����

	// �������ɫ����Ҫ���	
	if (p.color > 0)
	{
		// ����
		if (p.nPointsNum == 1)
		{
			DrawPixel(pPoints[0].x, pPoints[0].y, p.color);
		}
		else
		{
			// ���ض�������ɫ
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

	// �������ɫ�������ƶ���Σ�������Ҫ��������
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

/** @brief �Ƚ� Polygon3D ����� */
inline bool compPolygonDepth(const Polygon3D a, const Polygon3D b)
{
	return a.pPoints[0].z > b.pPoints[0].z;
}

/**
 * @brief	�������ȵĿ��������������һ�����غ�����
 * @note	���Բ���̳�
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
 * @brief			�������ȵĿ�������
 * @param[in/out]	s: ���������
 * @param[in]		size: �����С
 * @return			������ֵ������Ľ�������������
*/
inline void SortPolygons(Polygon3D *s, int size)
{
	SortPolygons(s, 0, size - 1);
}

/** @brief 3D ���� */
class Object3D
{
private:

	Polygon3D* pPolygons;			/** @brief ����Ķ�������� */
	Polygon3D* pRotatedPolygons;	/** @brief �������̬������Ķ�������� */
	int nPolygonsNum;				/** @brief ����Ķ�������� */

	Point3D pCenter;		/** @brief �������ĵ� */
	Attitude3D attitude;	/** @brief ������̬ */
	int rotate_order[3];	/** @brief ��ת˳�� */

	/**
	 * @brief	�����������ĵ�λ��
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
	 * @brief		������ת�����鳤��
	 * @attention	������ԭ�����е�����
	*/
	void UpdateRotatedPointsArrayLength()
	{
		Polygon3D* newArray = new Polygon3D[nPolygonsNum];
		if (pRotatedPolygons)
			delete[] pRotatedPolygons;
		pRotatedPolygons = newArray;
	}

	/**
	 * @brief		����Ԫ������
	 * @attention	�����Ԫ��������������ʱ�����ô˺���
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

	/** @brief ������ת˳�� */
	void SetRotateOrder(int pOrder[3])
	{
		for (int i = 0; i < 3; i++)
			rotate_order[i] = pOrder[i];
	}

	/** @brief ��ȡ��ת˳�� */
	int* GetRotateOrder()
	{
		return rotate_order;
	}

	/** @brief ��ȡ�����е������ */
	int GetPointsNum()
	{
		int num = 0;
		for (int i = 0; i < nPolygonsNum; i++)
		{
			num += pPolygons[i].nPointsNum;
		}
		return num;
	}

	/** @brief ��ȡ�����ж���ε����� */
	int GetPolygonsNum()
	{
		return nPolygonsNum;
	}

	/**
	 * @brief		��ȡ��������е�
	 * @param[in]	rotated: �Ƿ��ȡ��ת��ĵ�
	 * @attention	���� GetPointsNum ��������ȡ�������
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
	 * @brief		��ȡ�����е����ж����
	 * @param[in]	rotated: �Ƿ��ȡ��ת��Ķ����
	 * @attention	���� GetPolygonsNum ��������ȡ����ε�����
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

	/** @brief ��ȡ�������ĵ����� */
	Point3D GetCenterPoint()
	{
		return pCenter;
	}

	/**  @brief	��ȡ����λ�ã�Ч���� GetCenterPoint ������ͬ */
	Point3D GetPosition()
	{
		return GetCenterPoint();
	}

	/** @brief �ƶ����嵽ĳλ�ã����������������� */
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

	/** @brief �������� x �᷽���ƶ������������������� */
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

	/** @brief �������� y �᷽���ƶ������������������� */
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

	/** @brief �������� z �᷽���ƶ������������������� */
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

	/** @brief ����������̬ */
	void SetAttitude(Attitude3D ati)
	{
		attitude = ati;
	}

	/** @brief ��ȡ������̬ */
	Attitude3D GetAttitude()
	{
		return attitude;
	}

	/** @brief �� x ��˳ʱ�����������̬ */
	void RotateX(double angle)
	{
		attitude.r += angle;
	}

	/** @brief �� y ��˳ʱ�����������̬ */
	void RotateY(double angle)
	{
		attitude.e += angle;
	}

	/** @brief �� z ��˳ʱ�����������̬ */
	void RotateZ(double angle)
	{
		attitude.a += angle;
	}

	/** @brief ��ȡ�������ά���α߿� */
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
	 * @brief		������ת�����飬���������̬���������
	 * @attention	���û����������̬�󲻻����������µ��������꣬��Ҫ�ֶ�����
	 *				�˺������м��㡣
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
	 * @brief		����������ӵ�
	 * @attention	�˺������Ե�����Ϊ��λ��ӣ�����ÿ���㶼������Ϊһ������β�������
	 * @param[in]	pNew: Ҫ��ӵĵ������
	 * @param[in]	num: Ҫ��ӵĵ������
	 * @return		��������ӵģ���������������еĵ�һ������������������ʧ�ܷ��� -1
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
	 * @brief		����������Ӷ����
	 * @param[in]	pNew: Ҫ��ӵĶ���ε�����
	 * @param[in]	num: Ҫ��ӵĶ���ε�����
	 * @return		��������ӵģ������������������еĵ�һ��������
	 *				����������ʧ�ܷ��� -1
	 * @attention	������ӹ����и����� pNew �е� pPoints �ĵ�ַ��
	 *				���Բ�Ҫɾ�� pNew ��Ӧ�� pPoints
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
	 * @brief		ɾ�������е�ĳ�������
	 * @param[in]	p: Ҫɾ���ĵ������
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

/** @brief 3D ���� */
class Scence3D
{

private:
	Object3D* pObjects;	/** @brief �����ڵ����弯�� */
	int nObjectsNum;	/** @brief �����ڵ��������� */

	Point3D pCamera;			/** @brief ������� */
	Attitude3D attitudeCamera;	/** @brief �����̬ */
	int nViewportWidth;			/** @brief ����ӿڿ�� */
	int nViewportHeight;		/** @brief ����ӿڸ߶� */
	int nFocalLength;			/** @brief ������� */


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

	/** @brief �������λ�� */
	void SetCameraPosition(Point3D p)
	{
		pCamera = p;
	}

	/** @brief ��ȡ���λ�� */
	Point3D GetCameraPosition()
	{
		return pCamera;
	}

	/** @brief �ƶ������Ч���� SetCameraPosition ��ͬ */
	void MoveCameraTo(Point3D p)
	{
		SetCameraPosition(p);
	}

	/** @brief �� x �᷽���ƶ���� */
	void MoveCameraX(double n)
	{
		pCamera.x += n;
	}

	/** @brief �� y �᷽���ƶ���� */
	void MoveCameraY(double n)
	{
		pCamera.y += n;
	}

	/** @brief �� z �᷽���ƶ���� */
	void MoveCameraZ(double n)
	{
		pCamera.z += n;
	}

	/** @brief ���������̬ */
	void SetCameraAttitude(Attitude3D ati)
	{
		attitudeCamera = ati;
	}

	/** @brief ��ȡ�����̬ */
	Attitude3D GetCameraAttitude()
	{
		return attitudeCamera;
	}

	/** @brief ������� x ����ת */
	void RotateCameraX(double angle)
	{
		attitudeCamera.r += angle;
	}

	/** @brief ������� y ����ת */
	void RotateCameraY(double angle)
	{
		attitudeCamera.e += angle;
	}

	/** @brief ������� z ����ת */
	void RotateCameraZ(double angle)
	{
		attitudeCamera.a += angle;
	}

	/** @brief ��������ӿڴ�С */
	void SetCameraViewportSize(int w, int h)
	{
		nViewportWidth = w;
		nViewportHeight = h;
	}

	/**
	 * @brief		��ȡ����ӿڴ�С
	 * @param[out]	w: �ӿڿ��
	 * @param[out]	h: �ӿڸ߶�
	*/
	void GetCameraViewportSize(int* w, int* h)
	{
		*w = nViewportWidth;
		*h = nViewportHeight;
	}

	/** @brief ����������� */
	void SetCameraFocalLength(int f)
	{
		nFocalLength = f;
	}

	/** @brief ��ȡ������� */
	int GetCameraFocalLength()
	{
		return nFocalLength;
	}

	/**
	 * @brief		�ڳ������������
	 * @param[in]	obj: Ҫ��ӵ�����
	 * @return		������ӵ������������е�����
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
	 * @brief		ɾ�������е�����
	 * @param[in]	pObj: Ҫɾ�������������
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

	/** @brief ��ȡ����������ļ��� */
	Object3D* GetObjects()
	{
		return pObjects;
	}

	/** @brief ��ȡ��������������� */
	int GetObjectsNum()
	{
		return nObjectsNum;
	}

	/** @brief ��ȡ���������е�����Ķ���ε������ܺ� */
	int GetPolygonsNum()
	{
		int index = 0;
		for (int i = 0; i < nObjectsNum; i++)
			index += pObjects[i].GetPolygonsNum();
		return index;
	}

	/** @brief ��ȡ��������������Ķ���εļ��� */
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
	 * @brief		���Ƴ�������Ļ
	 * @param[in]	offset_x: �������Ļ��ͼ��� x ����ƫ��
	 * @param[in]	offset_y: �������Ļ��ͼ��� y ����ƫ��
	 * @return		���ػ��ƺ�ʱ����λ���룩
	*/
	double Draw(int offset_x = 0, int offset_y = 0)
	{

		/*
		 *	����ӿ�
		 *
		 *		����ͼ
		 *
		 *            �� y ( viewport coordinate system )
		 *            |
		 *			_ | ____________________
		 *			�� +----------------------+
		 *			| |   |              |   |
		 *			| |   |              |   |
		 *	viewport| |   |______________|   |
		 *	height  | |  /       ��        \  |
		 *			| | /        | focal   \ |
		 *			| |/         �� length   \|
		 *			��[O]---------------------+ ��������������> x ( viewport coordinate system )
		 *		    / |������ viewport_width ������|
		 *			|
		 *			|
		 *			|_--�� viewport origin
		 *
		 *
		 *
		 *		����ͼ
		 *
		 *							    �� y ( viewport coordinate system )
		 *            __________________|__
		 *           /                  | /|
		 *			|��-- focal length -��|/ |   __  x ( viewport coordinate system )
		 *			+-------------------/  |    /\
		 *			|   |               |  |   /
		 *			|   |               |  | Oo
		 *			|   |               | }-[==] ----�� camera
		 *			|   |               |  |/
		 *			|___|_______________|  /
		 *	  <����������+------------------[O]-+
		 *   z ( viewport				|
		 *		 coordinate system )	|_----�� viewport origin
		 *
		 */

		TIMEC_INIT;

		int t = clock();

		printf("Ԥ��ʼ�� ");
		TIMEC_BEGIN;

		int nAllPolygonsNum = GetPolygonsNum();
		Polygon3D* pAllPolygons = GetPolygons();					// ���ж����
		Polygon3D* pFilterWide = new Polygon3D[nAllPolygonsNum];	// ��ɸѡ����Լ���ת����
		Polygon3D* pFilterNarrow = new Polygon3D[nAllPolygonsNum];	// ��ɸѡ���
		Polygon3D* pFilterDepth = new Polygon3D[nAllPolygonsNum];	// ���ɸѡ���
		int nFilterWideNum = 0, nFilterNarrowNum = 0, nFilterDepthNum = 0;

		int** pDepthMap = new int* [nViewportWidth];	// �������Ⱦ��Χ�ڵ�ÿ����������Ϣ
		for (int i = 0; i < nViewportWidth; i++)
		{
			// ��ʼ�������Ϣ�����ֵΪ���������ȼ��������࣬��һ�������ܳ�������Ⱦ��Χ�ڵ�ֵ
			pDepthMap[i] = new int[nViewportHeight];
			for (int j = 0; j < nViewportHeight; j++)
			{
				pDepthMap[i][j] = (int)pCamera.z + 2 * nFocalLength;
			}
		}

		// ѡȡ����ӿڵĽϳ��� (l)����ϳ��ߺͽ�����ɵ�ƽ��ĶԽ��߳��ȣ���Ϊ h
		int l = nViewportWidth > nViewportHeight ? nViewportWidth : nViewportHeight;
		int h = (int)sqrt(nFocalLength * nFocalLength + l * l);

		int lx = nViewportWidth / 2, ly = nViewportHeight / 2;					// limit_x and limit_y
		Point3D pOriginViewport = { pCamera.x - lx,pCamera.y - ly,pCamera.z };	// ����ӿڵ�ԭ��

		TIMEC_END;
		printf("��ɸѡ ");
		TIMEC_BEGIN;

		//// ��ʼɸѡ

		// ��ɸѡ
		for (int i = 0; i < nAllPolygonsNum; i++)
		{
			for (int j = 0; j < pAllPolygons[i].nPointsNum; j++)
			{
				// ���ö���ε���������һ�������������Ϊ���ĵġ��߳�Ϊ h ���������ڣ����п��ܱ���Ⱦ
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
		printf("��ת ");
		TIMEC_BEGIN;

		// �Է�ɸѡ���Ķ�����������ת����ȡ�����̬�£���Щ����ε�����
		for (int i = 0; i < nFilterWideNum; i++)
		{
			for (int j = 0; j < pFilterWide[i].nPointsNum; j++)
			{
				pFilterWide[i].pPoints[j] = Rotate3D(pFilterWide[i].pPoints[j], -attitudeCamera.a, -attitudeCamera.e, -attitudeCamera.r, pCamera);
			}
		}

		TIMEC_END;
		printf("ת������ϵ ");
		TIMEC_BEGIN;

		// ��ת������ת�� ����ӿ� ������ϵ
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
		printf("��ɸѡ ");
		TIMEC_BEGIN;

		// ��ɸѡ
		for (int i = 0; i < nFilterWideNum; i++)
		{
			for (int j = 0; j < pFilterWide[i].nPointsNum; j++)
			{
				// ���ö���ε�����һ���������������Ե��ӿ��������У���������Ŀ��Ϊ�ӿڿ�ߣ����Ϊ���ࣩ�����ж��˶�������������Ⱦ��Χ��
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
		printf("��¼�����Ϣ ");
		TIMEC_BEGIN;

		// �����Ϣ��¼
		for (int i = 0; i < nFilterNarrowNum; i++)
		{
			for (int j = 0; j < pFilterNarrow[i].nPointsNum; j++)
			{
				// ������ɸѡ������е㣬�ó����ӿ�ƽ���ϵ�ÿ�������ǳ���
				if (pFilterNarrow[i].pPoints[j].z < pDepthMap[(int)pFilterNarrow[i].pPoints[j].x][(int)pFilterNarrow[i].pPoints[j].y])
				{
					pDepthMap[(int)pFilterNarrow[i].pPoints[j].x][(int)pFilterNarrow[i].pPoints[j].y] = (int)pFilterNarrow[i].pPoints[j].z;
				}
			}
		}

		TIMEC_END;
		printf("���ɸѡ ");
		TIMEC_BEGIN;

		// ���ɸѡ��ɸ����������� z ���궼���ڶ���Ķ���Σ���Ϊ���ǵ���Ⱦ����Ч��
		for (int i = 0; i < nFilterNarrowNum; i++)
		{
			for (int j = 0; j < pFilterNarrow[i].nPointsNum; j++)
			{
				// �ö���ε���һ������ z ��Ķ���
				if (pFilterNarrow[i].pPoints[j].z <= pDepthMap[(int)pFilterNarrow[i].pPoints[j].x][(int)pFilterNarrow[i].pPoints[j].y])
				{
					pFilterDepth[nFilterDepthNum] = pFilterNarrow[i];
					nFilterDepthNum++;
					break;
				}
			}
		}

		TIMEC_END;
		printf("���� ");
		TIMEC_BEGIN;

		// �������ɸѡ����ɸѡ����� z ���������Ծ�������˳��
		SortPolygons(pFilterDepth, nFilterDepthNum);

		TIMEC_END;
		printf("���� ");
		TIMEC_BEGIN;

		// ����
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
