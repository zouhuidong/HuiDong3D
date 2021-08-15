/**
 * @file	HuiDong3D.h
 * @brief	3D ������ͷ�ļ�
 *
 * @author	huidong
 * @mail	huidong_mail@163.com
 * @version	Ver 0.1.2 (beta)
 * @date	����ʱ�� 2021.8.2
 *			����޸� 2021.8.15
 *
 */

#pragma once

#include <math.h>
#include <time.h>
#include <algorithm>

 // ͼ�ο�
#include <easyx.h>
#include <stdio.h>

#define _HD3D_BEGIN	namespace HD3D {
#define _HD3D_END	}

_HD3D_BEGIN

//////// �궨��

/**
 * @brief �����������
*/
#define POLYGON_MAX_SIDES 16

/**
 * @brief ��Ⱦ���ѵ�����ʱ�䣨�룩
 * @note ��ֵֻ�������������Ϊȡֵ�����ܸı�������Ⱦʱ��
*/
#define MIN_TIME_COST 1.0 / 9999

//////// �������Ͷ���

/**
 * @brief ��ɫ��Ϣ��Ϊ�������ʾ����ɫ��
*/
typedef int Color;

/**
 * @brief 2D ����
*/
struct Point2D
{
	double x;
	double y;
};

/**
 * @brief 3D ����
*/
struct Point3D
{
	double x;
	double y;
	double z;
};

/**
 * @brief ������ɫ�� 3D ����
*/
struct ColorPoint3D
{
	double x;
	double y;
	double z;
	Color color;

	/**
	 * @brief ת��Ϊ������ɫ�� 3D ����
	*/
	operator Point3D() const { return { x,y,z }; }
};

/**
 * @brief 3D �����壬�洢�������������С�� x,y,z ֵ
*/
struct Rectangle3D
{
	double min_x, min_y, min_z, max_x, max_y, max_z;
};

/**
 * @brief ��������
*/
struct Zoom
{
	double x;	/** @brief x ������������ */
	double y;	/** @brief y ������������ */
};

/**
 * @brief 3D �����
*/
class Polygon3D
{
private:

	void init()
	{
		pPoints = new Point3D[POLYGON_MAX_SIDES];
		memset(pPoints, 0, sizeof Point3D * POLYGON_MAX_SIDES);
		nPointsNum = 0;
		color = 0;
	}

public:

	Polygon3D()
	{
		init();
	}

	/**
	 * @brief ���ݶ��㴴�������
	 * @param[in] p : ��������
	 * @param[in] n : ��������
	 * @param[in] c : ����������ɫ
	 * @attention �����������ó�����󶥵�����
	 * @see POLYGON_MAX_SIDES
	*/
	Polygon3D(const Point3D* p, int n, Color c)
	{
		init();
		for (int i = 0; i < n; i++)
			pPoints[i] = p[i];
		nPointsNum = n;
		color = c;
	}

	/*~Polygon3D()
	{
		clear();
	}*/

	bool operator< (Polygon3D p)
	{
		return GetCenterZ() < p.GetCenterZ();
	}

	bool operator>= (Polygon3D p)
	{
		return GetCenterZ() >= p.GetCenterZ();
	}

	/**
	 * @brief ��ȡ�˶���ε� z ��ε�����
	*/
	double GetCenterZ()
	{
		if (nPointsNum <= 0) return 0;
		double min = pPoints[0].z, max = pPoints[0].z;
		for (int i = 0; i < nPointsNum; i++)
		{
			if (pPoints[i].z < min)
			{
				min = pPoints[i].z;
			}
			else if (pPoints[i].z > max)
			{
				max = pPoints[i].z;
			}
		}
		return min + (max - min) / 2;
	}

	/**
	 * @brief ������ݣ��ͷ��ڴ�
	*/
	void clear()
	{
		if (pPoints) delete[] pPoints;
		pPoints = NULL;
	}

	Point3D* pPoints;	/** @brief ����ζ��� */
	int nPointsNum;		/** @brief ����ζ������� */
	Color color;		/** @brief ����������ɫ */
};

/**
 * @brief ɾ�����������
 * @param p : ���������
 * @param num : ���鳤��
 * @note �Դ��Կ�������ж�
*/
inline void DeletePolygons(Polygon3D*& p, int num)
{
	if (!p)
		return;
	for(int i=0;i<num;i++)
		p[i].clear();
	delete[] p;
	p = NULL;
}

/**
 * @brief ���ƶ��������
 * @param pDst : Ŀ������
 * @param pSrc : Դ����
 * @param num : Դ���鳤��
 * @return ����Ŀ�������ָ�룬�����һ����Ϊ���򷵻� NULL
*/
inline Polygon3D* CopyPolygons(Polygon3D* pDst, Polygon3D* pSrc, int num)
{
	if (!pDst || !pSrc)
	{
		return NULL;
	}
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < pSrc[i].nPointsNum; j++)
		{
			pDst[i].pPoints[j] = pSrc[i].pPoints[j];
		}
		pDst[i].nPointsNum = pSrc[i].nPointsNum;
		pDst[i].color = pSrc[i].color;
	}
	return pDst;
}

/**
 * @brief 3D �������̬
 * @attention ��̬����ת�Ƕȶ��ǻ�����������ģ������������תԭ��
 * @note ���½Ƕȶ�����ʱ����Ϊ������ת����
 */
struct Attitude3D
{
	double a;	/** @brief ��������� z ����ת�� */
	double e;	/** @brief ��������� y ����ת�� */
	double r;	/** @brief ��������� x ����ת�� */
};

/**
 * @brief ��ת˳��
*/
enum RotateOrder { rotate_x, rotate_y, rotate_z };

/**
 * @brief Ĭ����ת˳��
*/
int m_defaultRotateOrder[3] = { rotate_z,rotate_y,rotate_x };

//////// ��������ʵ��

/**
 * @brief ��������ɫ�� 3D ����ת��Ϊ����ɫ�� 3D ����
*/
inline ColorPoint3D ToColorPoint3D(Point3D p)
{
	return { p.x,p.y,p.z,(Color)0 };
}

/**
 * @brief ���Ƕ�ת��Ϊ����
 * @param[in] angle : ԭ�Ƕ�
 * @return �Ƕ�ת����õ��Ļ���
*/
inline double ConvertToRadian(double angle)
{
	return 3.1415926535 / 180.0 * angle;
}

/**
 * @brief ��ת 2D ����
 * @param[in] x : x ����
 * @param[in] y : y ����
 * @param[in] angle : ��ʱ����ת�Ƕȣ�Ϊ�������ʾ˳ʱ����ת
 * @return ��ת��� 2D ����
*/
inline Point2D Rotate2D(double x, double y, double angle)
{
	if (angle == 0)	return { x,y };
	double t = ConvertToRadian(angle);
	return { x * cos(t) - y * sin(t),x * sin(t) + y * cos(t) };
}

/**
 * @brief ˳ʱ����ת 2D ����
 * @param[in] p : Ҫ��ת�� 2D ����
 * @param[in] angle : ��ת�Ƕ�
 * @return ��ת��� 2D ����
*/
inline Point2D Rotate2D(Point2D p, double angle)
{
	return Rotate2D(p.x, p.y, angle);
}

/**
 * @brief �� 3D ������ X ��˳ʱ����ת
 * @param[in] p : Ҫ��ת������
 * @param[in] angle : ��ת�Ƕ�
 * @param[in] pOrigin : ��תԭ��
 * @return ��ת�������
*/
inline Point3D Rotate3D_X(Point3D p, double angle, Point3D pOrigin = { 0,0,0 })
{
	Point2D p2 = Rotate2D({ p.y - pOrigin.y,p.z - pOrigin.z }, angle);
	return { p.x,p2.x + pOrigin.y,p2.y + pOrigin.z };
}

/**
 * @brief �� 3D ������ Y ��˳ʱ����ת
 * @param[in] p : Ҫ��ת������
 * @param[in] angle : ��ת�Ƕ�
 * @param[in] pOrigin : ��תԭ��
 * @return ��ת�������
*/
inline Point3D Rotate3D_Y(Point3D p, double angle, Point3D pOrigin = { 0,0,0 })
{
	Point2D p2 = Rotate2D({ p.x - pOrigin.x,p.z - pOrigin.z }, angle);
	return { p2.x + pOrigin.x,p.y,p2.y + pOrigin.z };
}

/**
 * @brief �� 3D ������ Z ��˳ʱ����ת
 * @param[in] p : Ҫ��ת������
 * @param[in] angle : ��ת�Ƕ�
 * @param[in] pOrigin : ��תԭ��
 * @return ��ת�������
*/
inline Point3D Rotate3D_Z(Point3D p, double angle, Point3D pOrigin = { 0,0,0 })
{
	Point2D p2 = Rotate2D({ p.x - pOrigin.x,p.y - pOrigin.y }, angle);
	return { p2.x + pOrigin.x,p2.y + pOrigin.y,p.z };
}

/**
 * @brief ��ת 3D ����
 * @param[in] p : ԭ����
 * @param[in] a : �� z �����ת�Ƕ�
 * @param[in] e : �� y �����ת�Ƕ�
 * @param[in] r : �� x �����ת�Ƕ�
 * @param[in] pOrigin : ��תԭ��
 * @param[in] pOrder : ��ת˳��
 * @return ��ת�������
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
 * @brief ������μ������������ת
 * @param[in] pPolygons : ����μ���
 * @param[in] num : ���������
 * @param[in] atiCamera : �����̬
 * @param[in] pCamera : ���λ��
 * @return ������ת��Ķ���μ���
*/
inline Polygon3D* RotateToCamera(Polygon3D* pPolygons, int num, Attitude3D atiCamera, Point3D pCamera)
{
	Polygon3D* pRotated = new Polygon3D[num];
	CopyPolygons(pRotated,pPolygons,num);

	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < pRotated[i].nPointsNum; j++)
		{
			pRotated[i].pPoints[j] = Rotate3D(pRotated[i].pPoints[j], -atiCamera.a, -atiCamera.e, -atiCamera.r, pCamera);
		}
	}
	return pRotated;
}

/**
 * @brief ������μ���ת������ϵ
 * @param[in] pPolygons : ����ϵ����
 * @param[in] num : ���������
 * @param[in] pOrigin : �µ�����ϵ��ԭ���ھ�����ϵ�е�λ��
 * @return ����ת������ϵ��Ķ���μ���
*/
inline Polygon3D* ConvertCoordinateSystem(Polygon3D* pPolygons, int num, Point3D pOrigin)
{
	Polygon3D* pConverted = new Polygon3D[num];
	CopyPolygons(pConverted, pPolygons, num);

	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < pConverted[i].nPointsNum; j++)
		{
			pConverted[i].pPoints[j].x -= pOrigin.x;
			pConverted[i].pPoints[j].y -= pOrigin.y;
			pConverted[i].pPoints[j].z -= pOrigin.z;
		}
	}
	return pConverted;
}

/**
 * @brief ������μ��ϼ���͸��ͶӰЧ��
 * @param[in] pPolygons : ����μ���
 * @param[in] num : ���������
 * @param[in] nFocal : ����
 * @return ����͸��ͶӰ��Ķ���μ���
 * @attention ͸�����ĵ�������ϵԭ��
 * @note ������ӿ���ĵ���вü�
*/
inline Polygon3D* GetPerspectiveProjectionPolygons(Polygon3D* pPolygons, int num, int nFocal)
{
	Polygon3D* pProjection = new Polygon3D[num];
	CopyPolygons(pProjection, pPolygons, num);

	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < pProjection[i].nPointsNum; j++)
		{
			double zoom = (nFocal - pPolygons[i].pPoints[j].z) / nFocal;
			pProjection[i].pPoints[j].x *= zoom;
			pProjection[i].pPoints[j].y *= zoom;
		}
	}
	return pProjection;
}

/**
 * @brief �ü� NDC ����μ���
 * @param[in] pPolygons : NDC ����μ���
 * @param[in] num : ���������
 * @param[in] nFocal : ����
 * @param[out] out_count : ���زü�����������
 * @return ���زü���Ķ���μ���
*/
inline Polygon3D* CropNDCPolygons(Polygon3D* pPolygons, int num, int nFocal, int* out_count)
{

	/////////// ��ʱ������һ���Ƚϼ򵥴ֱ����ж�
	/////////// ����ֻ���������ӿ�������Ĳü� (x[-1 ~ 1]  y [-1 ~ 1]  z[0 ~ f])
	/////////// ����Ҫ�ĳɶ���ν����ж�

	Polygon3D* pCrop = new Polygon3D[num];

	int count = 0;
	for (int i = 0; i < num; i++)
	{
		int crop_count = 0;
		for (int j = 0; j < pPolygons[i].nPointsNum; j++)
		{
			// ͳ��һ������γ����ӿڵĵ�ĸ���
			if (pPolygons[i].pPoints[j].x < -1 || pPolygons[i].pPoints[j].x > 1 ||
				pPolygons[i].pPoints[j].y < -1 || pPolygons[i].pPoints[j].y > 1 ||
				pPolygons[i].pPoints[j].z < 0 || pPolygons[i].pPoints[j].z > nFocal)
			{
				crop_count++;
			}

			// ������һ�������ӿ��ھͿ��Ա�����
			else
			{
				break;
			}
		}

		// �������е㶼�����ӿڵ�����²ű����˶���Σ����򱻲ü���
		if (crop_count < pPolygons[i].nPointsNum)
		{
			CopyPolygons(&pCrop[count], &pPolygons[i], 1);	
			count ++;
		}
	}

	// ��������������
	Polygon3D* pReserve = new Polygon3D[count];
	CopyPolygons(pReserve, pCrop, count);

	// �ͷŴ����������
	DeletePolygons(pCrop, num);

	*out_count = count;
	return pReserve;
}

//////// ��ͼ�豸���

/**
 * @brief ��ʼ����ͼ�豸
 * @param[in] w : ��ͼ�豸��
 * @param[in] w : ��ͼ�豸��
 * @param[in] flag : �������ԣ�Ĭ��Ϊ 0
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
 * @brief ��ȡ��ͼ�豸���
*/
inline int GetDrawingDeviceWidth()
{
	return getwidth();
}

/**
 * @brief ��ȡ��ͼ�豸�߶�
*/
inline int GetDrawingDeviceHeight()
{
	return getheight();
}

/**
 * @brief ���Ƶ�
 * @param[in] x : ����λ��
 * @param[in] y : ����λ��
 * @param[in] c : ������ɫ
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
 * @brief ���Ƶ�
 * @param[in] p : ����λ��
 * @param[in] c : ������ɫ
*/
inline void DrawPixel(Point2D p, Color c)
{
	DrawPixel((int)p.x, (int)p.y, c);
}

/**
 * @brief �� 3D �� NDC ���� תΪ 3D ����Ļ����
 * @param[in] p : ԭ����
 * @param[in] zoom : ������������
 * @return ����ת�������Ļ����
 * @note Ĭ�Ϲ۲���Ϊ xoy
*/
inline Point3D ConvertNDC3DToScreenPoint(Point3D p, Zoom zoom = { 1,1 })
{
	return { (p.x * zoom.x + 1) * GetDrawingDeviceWidth(),(1 - p.y * zoom.y) * GetDrawingDeviceHeight() };
}

/**
 * @brief �����������
 * @param[in] p : 3D �����
 * @param[in] offset_x : �����ͼ��� x ����ƫ��
 * @param[in] offset_y : �����ͼ��� y ����ƫ��
 * @param[in] zoom : ͼ����������
 * @param[in] grid : �����������ɫ��Ϊ������ʾ����������
 * @attention ֻȡ����ε� x,y ������Ƶ���Ļ
*/
inline void DrawFillPolygon(Polygon3D p, int offset_x = 0, int offset_y = 0, Zoom zoom = { 1,1 }, Color grid = -1)
{
	if (p.nPointsNum <= 0) return;

	POINT* pPoints = new POINT[p.nPointsNum];
	for (int j = 0; j < p.nPointsNum; j++)
	{
		Point3D pp = ConvertNDC3DToScreenPoint(p.pPoints[j], zoom);
		pPoints[j] = { (long)(pp.x) + offset_x,(long)(pp.y) + offset_y };
	}

	//// easyx ����

	// �������ɫ����Ҫ���	
	if (p.color >= 0)
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
			setfillcolor(WHITE);
			polygon(pPoints, p.nPointsNum);
		}
	}

	delete[] pPoints;
}

//////// �ඨ��

/**
 * @brief 3D ����
*/
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
		Rectangle3D r = GetRectangle(false);

		pCenter = {
			(r.max_x - r.min_x) / 2 + r.min_x,
			(r.max_y - r.min_y) / 2 + r.min_y,
			(r.max_z - r.min_z) / 2 + r.min_z
		};
	}

	/**
	 * @brief ������ת�����鳤��
	 * @attention ������ԭ�����е�����
	*/
	void UpdateRotatedPointsArrayLength()
	{
		Polygon3D* newArray = new Polygon3D[nPolygonsNum];
		DeletePolygons(pRotatedPolygons, nPolygonsNum);
		pRotatedPolygons = newArray;
	}

	/**
	 * @brief ����Ԫ������
	 * @attention �����Ԫ��������������ʱ�����ô˺���
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
		Polygon3D* p = NULL;
		for (int i = 0; i < 2; i++)
		{
			if (i == 0) p = pPolygons;
			else p = pRotatedPolygons;
			DeletePolygons(p, nPolygonsNum);
		}
	}

	/**
	 * @brief ������ת˳��
	*/
	void SetRotateOrder(int pOrder[3])
	{
		for (int i = 0; i < 3; i++)
			rotate_order[i] = pOrder[i];
	}

	/**
	 * @brief ��ȡ��ת˳��
	*/
	int* GetRotateOrder()
	{
		return rotate_order;
	}

	/**
	 * @brief ��ȡ�����е������
	*/
	int GetPointsNum()
	{
		int num = 0;
		for (int i = 0; i < nPolygonsNum; i++)
		{
			num += pPolygons[i].nPointsNum;
		}
		return num;
	}

	/**
	 * @brief ��ȡ�����ж���ε�����
	*/
	int GetPolygonsNum()
	{
		return nPolygonsNum;
	}

	/**
	 * @brief ��ȡ��������е�
	 * @param[in] rotated : �Ƿ��ȡ��ת��ĵ�
	 * @attention ���� GetPointsNum ��������ȡ�������
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
	 * @brief ��ȡ�����е����ж����
	 * @param[in] rotated : �Ƿ��ȡ��ת��Ķ����
	 * @attention ���� GetPolygonsNum ��������ȡ����ε�����
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

	/**
	 * @brief ��ȡ�������ĵ�����
	*/
	Point3D GetCenterPoint()
	{
		return pCenter;
	}

	/**
	 * @brief ��ȡ����λ�ã�Ч���� GetCenterPoint ������ͬ
	*/
	Point3D GetPosition()
	{
		return GetCenterPoint();
	}

	/**
	 * @brief �ƶ����嵽ĳλ�ã�����������������
	*/
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

	/**
	 * @brief �������� x �᷽���ƶ�������������������
	*/
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

	/**
	 * @brief �������� y �᷽���ƶ�������������������
	*/
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

	/**
	 * @brief �������� z �᷽���ƶ�������������������
	*/
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

	/**
	 * @brief ����������̬
	*/
	void SetAttitude(Attitude3D ati)
	{
		attitude = ati;
	}

	/**
	 * @brief ��ȡ������̬
	*/
	Attitude3D GetAttitude()
	{
		return attitude;
	}

	/**
	 * @brief �� x ��˳ʱ�����������̬
	*/
	void RotateX(double angle)
	{
		attitude.r += angle;
	}

	/**
	 * @brief �� y ��˳ʱ�����������̬
	*/
	void RotateY(double angle)
	{
		attitude.e += angle;
	}

	/**
	 * @brief �� z ��˳ʱ�����������̬
	*/
	void RotateZ(double angle)
	{
		attitude.a += angle;
	}

	/**
	 * @brief ��ȡ�������ά���α߿�
	 * @param[in] bRotated : �Ƿ��ȡ�Ѿ���ת������ı߿�
	 * @return ��������߿�
	*/
	Rectangle3D GetRectangle(bool bRotated = true)
	{
		Rectangle3D r = {};
		Polygon3D* p = pPolygons;
		if(bRotated && pRotatedPolygons)
			p = pRotatedPolygons;
		for (int i = 0; i < nPolygonsNum; i++)
		{
			for (int j = 0; j < p[i].nPointsNum; j++)
			{
				// init
				if (i + j == 0)
				{
					r.min_x = r.max_x = p[i].pPoints[j].x;
					r.min_y = r.max_y = p[i].pPoints[j].y;
					r.min_z = r.max_z = p[i].pPoints[j].z;
					continue;
				}

				if (p[i].pPoints[j].x < r.min_x)
					r.min_x = p[i].pPoints[j].x;
				else if (p[i].pPoints[j].x > r.max_x)
					r.max_x = p[i].pPoints[j].x;

				if (p[i].pPoints[j].y < r.min_y)
					r.min_y = p[i].pPoints[j].y;
				else if (p[i].pPoints[j].y > r.max_y)
					r.max_y = p[i].pPoints[j].y;

				if (p[i].pPoints[j].z < r.min_z)
					r.min_z = p[i].pPoints[j].z;
				else if (p[i].pPoints[j].z > r.max_z)
					r.max_z = p[i].pPoints[j].z;
			}
		}
		return r;
	}

	/**
	 * @brief ������ת�����飬���������̬���������
	 * @attention ���û����������̬�󲻻����������µ��������꣬��Ҫ�ֶ�����
	 *			�˺������м��㡣
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
	 * @brief ����������ӵ�
	 * @attention �˺������Ե�����Ϊ��λ��ӣ�����ÿ���㶼������Ϊһ������β�������
	 * @param[in] pNew : Ҫ��ӵĵ������
	 * @param[in] num : Ҫ��ӵĵ������
	 * @return ��������ӵģ���������������еĵ�һ������������������ʧ�ܷ��� -1
	*/
	int AddPoints(ColorPoint3D* pNew, int num)
	{
		if (num <= 0 || !pNew)	return -1;

		Polygon3D* newArray = new Polygon3D[nPolygonsNum + num];
		CopyPolygons(newArray, pPolygons, nPolygonsNum);

		for (int i = nPolygonsNum, j = 0; j < num; i++, j++)
		{
			newArray[i].pPoints[0] = pNew[j];
			newArray[i].nPointsNum = 1;
			newArray[i].color = pNew[j].color;
		}

		DeletePolygons(pPolygons, nPolygonsNum);

		pPolygons = newArray;
		nPolygonsNum += num;

		UpdateArray();

		return nPolygonsNum - num;
	}

	/**
	 * @brief ����������Ӷ����
	 * @param[in] pNew : Ҫ��ӵĶ���ε�����
	 * @param[in] num : Ҫ��ӵĶ���ε�����
	 * @return ��������ӵģ������������������еĵ�һ������������������ʧ�ܷ��� -1
	*/
	int AddPolygons(Polygon3D* pNew, int num)
	{
		if (num <= 0 || !pNew)	return -1;

		Polygon3D* newArray = new Polygon3D[nPolygonsNum + num];
		CopyPolygons(newArray, pPolygons, nPolygonsNum);
		CopyPolygons(newArray+ nPolygonsNum, pNew, num);

		DeletePolygons(pPolygons, nPolygonsNum);

		pPolygons = newArray;
		nPolygonsNum += num;

		UpdateArray();

		return nPolygonsNum - num;
	}

	/**
	 * @brief ɾ�������е�ĳ�������
	 * @param[in] p : Ҫɾ���ĵ������
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

		DeletePolygons(pPolygons, nPolygonsNum);

		pPolygons = newArray;
		nPolygonsNum--;

		UpdateArray();
	}

};


/**
 * @brief 3D ����
*/
class Scence3D
{

private:
	Object3D* pObjects;	/** @brief �����ڵ����弯�� */
	int nObjectsNum;	/** @brief �����ڵ��������� */

	Point3D pCamera;			/** @brief ������� */
	Attitude3D attitudeCamera;	/** @brief �����̬ */
	int nViewportWidth;			/** @brief ����ӿڿ�� */
	int nViewportHeight;		/** @brief ����ӿڸ߶� */

	/**
	 * @brief �������
	 * @attention Ϊ�˻����������ʾ��������ò�С�� 600
	*/
	int nFocalLength;

	bool bPerspectiveProjection;	/** @brief �Ƿ�ʹ��͸��ͶӰ */

public:

	Scence3D()
	{
		pObjects = NULL;
		nObjectsNum = 0;

		pCamera = { 0,0,0 };
		attitudeCamera = { 0,0,0 };

		nViewportWidth = 640;
		nViewportHeight = 480;
		nFocalLength = 1000;

		bPerspectiveProjection = true;
	}

	~Scence3D()
	{
		if (pObjects) delete[] pObjects;
	}

	/**
	 * @brief �������λ��
	*/
	void SetCameraPosition(Point3D p)
	{
		pCamera = p;
	}

	/**
	 * @brief ��ȡ���λ��
	*/
	Point3D GetCameraPosition()
	{
		return pCamera;
	}

	/**
	 * @brief �ƶ������Ч���� SetCameraPosition ��ͬ
	*/
	void MoveCameraTo(Point3D p)
	{
		SetCameraPosition(p);
	}

	/**
	 * @brief �� x �᷽���ƶ����
	*/
	void MoveCameraX(double n)
	{
		pCamera.x += n;
	}

	/**
	 * @brief �� y �᷽���ƶ����
	*/
	void MoveCameraY(double n)
	{
		pCamera.y += n;
	}

	/**
	 * @brief �� z �᷽���ƶ����
	*/
	void MoveCameraZ(double n)
	{
		pCamera.z += n;
	}

	/**
	 * @brief ���������̬
	*/
	void SetCameraAttitude(Attitude3D ati)
	{
		attitudeCamera = ati;
	}

	/**
	 * @brief ��ȡ�����̬
	*/
	Attitude3D GetCameraAttitude()
	{
		return attitudeCamera;
	}

	/**
	 * @brief ������� x ����ת
	*/
	void RotateCameraX(double angle)
	{
		attitudeCamera.r += angle;
	}

	/**
	 * @brief ������� y ����ת
	*/
	void RotateCameraY(double angle)
	{
		attitudeCamera.e += angle;
	}

	/**
	 * @brief ������� z ����ת
	*/
	void RotateCameraZ(double angle)
	{
		attitudeCamera.a += angle;
	}

	/**
	 * @brief ��������ӿڴ�С
	*/
	void SetCameraViewportSize(int w, int h)
	{
		nViewportWidth = w;
		nViewportHeight = h;
	}

	/**
	 * @brief ��ȡ����ӿڴ�С
	 * @param[out]	w : �ӿڿ��
	 * @param[out]	h : �ӿڸ߶�
	*/
	void GetCameraViewportSize(int* w, int* h)
	{
		*w = nViewportWidth;
		*h = nViewportHeight;
	}

	/**
	 * @brief �����������
	*/
	void SetCameraFocalLength(int f)
	{
		nFocalLength = f;
	}

	/**
	 * @brief ��ȡ�������
	*/
	int GetCameraFocalLength()
	{
		return nFocalLength;
	}

	/**
	 * @brief ����͸��ͶӰ�Ŀ���
	 * @param b : �Ƿ���͸��ͶӰ
	*/
	void EnablePerspectiveProjection(bool b = true)
	{
		bPerspectiveProjection = b;
	}

	/**
	 * @brief ��ȡ͸��ͶӰ״̬
	*/
	bool GetPerspectiveProjectionState()
	{
		return bPerspectiveProjection;
	}

	/**
	 * @brief �ڳ������������
	 * @param[in] obj : Ҫ��ӵ�����
	 * @return ������ӵ������������е�����
	*/
	int AddObject(Object3D& obj)
	{
		Object3D* newObjects = new Object3D[nObjectsNum + 1];
		for (int i = 0; i < nObjectsNum; i++)
		{
			newObjects[i].AddPolygons(pObjects[i].GetPolygons(), pObjects[i].GetPolygonsNum());
			newObjects[i].SetAttitude(pObjects[i].GetAttitude());
			newObjects[i].SetRotateOrder(pObjects[i].GetRotateOrder());
		}

		newObjects[nObjectsNum].AddPolygons(obj.GetPolygons(), obj.GetPolygonsNum());
		newObjects[nObjectsNum].SetAttitude(obj.GetAttitude());
		newObjects[nObjectsNum].SetRotateOrder(obj.GetRotateOrder());

		if (pObjects) delete[] pObjects;
		pObjects = newObjects;
		nObjectsNum++;
		return nObjectsNum - 1;
	}

	/**
	 * @brief ɾ�������е�����
	 * @param[in] pObj : Ҫɾ�������������
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

	/**
	 * @brief ��ȡ����������ļ���
	*/
	Object3D* GetObjects()
	{
		return pObjects;
	}

	/**
	 * @brief ��ȡ���������������
	*/
	int GetObjectsNum()
	{
		return nObjectsNum;
	}

	/**
	 * @brief ��ȡ���������е�����Ķ���ε������ܺ�
	*/
	int GetAllPolygonsNum()
	{
		int index = 0;
		for (int i = 0; i < nObjectsNum; i++)
			index += pObjects[i].GetPolygonsNum();
		return index;
	}

	/**
	 * @brief ��ȡ��������������Ķ���εļ���
	*/
	Polygon3D* GetAllPolygons()
	{
		Polygon3D* p = new Polygon3D[GetAllPolygonsNum()];
		for (int i = 0, index = 0; i < nObjectsNum; i++)
		{
			Polygon3D* pp = pObjects[i].GetPolygons();
			int num = pObjects[i].GetPolygonsNum();
			CopyPolygons(p + index, pp, num);
			index += num;
		}
		return p;
	}

	/**
	 * @brief ��ȡ�ӿ�����ϵ�µĶ���μ��ϣ�ƽ��ͶӰ��
	 * @param[out] count : ���ض��������
	*/
	Polygon3D* GetViewportPolygons(int* count = NULL)
	{
		int nAllPolygonsNum = GetAllPolygonsNum();
		if (nAllPolygonsNum <= 0) return NULL;

		Polygon3D* pAllPolygons = GetAllPolygons();
		Polygon3D* pRotated = NULL;
		Polygon3D* pConverted = NULL;

		// ����ӿڵ�ԭ��
		Point3D pOriginViewport = { pCamera.x - nViewportWidth / 2,pCamera.y - nViewportHeight / 2,pCamera.z };

		// ��ת������ӽǣ�����ƽ�ƣ�
		pRotated = RotateToCamera(pAllPolygons, nAllPolygonsNum, attitudeCamera, pCamera);

		// ƽ�Ƶ��ӿ�����ϵ
		pConverted = ConvertCoordinateSystem(pRotated, nAllPolygonsNum, pOriginViewport);

		DeletePolygons(pAllPolygons, nAllPolygonsNum);
		DeletePolygons(pRotated, nAllPolygonsNum);
		
		return pConverted;
	}

	/**
	 * @brief ��ȡ GetViewportPolygons �������صĶ���μ��ϵı�׼���豸���꣨NDC����ʽ�Ķ���μ���
	*/
	Polygon3D* GetViewportNDCPolygons()
	{
		int nPolygonsNum = GetAllPolygonsNum();
		if (nPolygonsNum <= 0) return NULL;
		Polygon3D* pPolygons = GetViewportPolygons();
		Polygon3D* pConverted = ConvertCoordinateSystem(pPolygons, nPolygonsNum, { nViewportWidth / 2.0,nViewportHeight / 2.0,0 });

		for (int i = 0; i < nPolygonsNum; i++)
		{
			for (int j = 0; j < pConverted[i].nPointsNum; j++)
			{
				pConverted[i].pPoints[j].x /= nViewportWidth / 2.0;
				pConverted[i].pPoints[j].y /= nViewportHeight / 2.0;
			}
		}

		DeletePolygons(pPolygons, nPolygonsNum);
		
		return pConverted;
	}

	/**
	 * @brief ��ȡҪ��Ⱦ�Ķ���μ���
	 * @param[out] count : ����Ҫ��Ⱦ�Ķ��������
	 * @return ��������Ⱦ��Χ�ڵĶ���μ��ϣ����Ѱ� z ������������
	 * @note ʹ�ô˺������Ի�ȡ����Ҫ���Ƶ��豸�Ķ���μ���
	*/
	Polygon3D* GetRenderPolygons(int* count)
	{
		int nPolygonsNum = GetAllPolygonsNum();
		int nCropNum = 0;
		if (nPolygonsNum <= 0) return NULL;
		Polygon3D* pPolygons = GetViewportNDCPolygons();
		Polygon3D* pCrop = NULL;
		Polygon3D* pShow = NULL;
		
		// ����͸��ͶӰ�Ļ��ͽ��м���
		if (bPerspectiveProjection)
		{
			////////////////////////////////////////////////////////
			// ͸��ͶӰʱԶ������Ť���� bug ����ʱ�Խ��������        //
			// ͸�ӵ�ʱ��ʹ�ö������࣬Ȼ��ü���ʱ��ֻ�ü���һ������  //
			////////////////////////////////////////////////////////

			pCrop = CropNDCPolygons(pPolygons, nPolygonsNum, nFocalLength * 2, &nCropNum);
			pShow = GetPerspectiveProjectionPolygons(pCrop, nCropNum, nFocalLength * 2);
			Polygon3D* pCrop2 = CropNDCPolygons(pShow, nCropNum, nFocalLength, &nCropNum);

			DeletePolygons(pCrop, nCropNum);
			DeletePolygons(pShow, nCropNum);

			pShow = pCrop2;
		}
		else
		{
			pCrop = CropNDCPolygons(pPolygons, nPolygonsNum, nFocalLength, &nCropNum);
			pShow = pCrop;
		}

		// ����� z ��������
		std::sort(pShow, pShow + nCropNum);

		DeletePolygons(pPolygons, nPolygonsNum);
		*count = nCropNum;
		return pShow;
	}

	/**
	 * @brief ���Ƴ�������Ļ
	 * @param[in] x : ͼ���������Ļ�� x ����
	 * @param[in] y : ͼ���������Ļ�� y ����
	 * @param[in] zoom : ͼ����������
	 * @param[in] grid : �����������ɫ��Ϊ������ʾ����������
	 * @return ���ػ��ƺ�ʱ����λ���룩
	*/
	double Render(int x = 0, int y = 0, Zoom zoom = { 1,1 }, Color grid = -1)
	{
		int t = clock();

		int nPolygonsNum = 0;
		Polygon3D* pPolygons = GetRenderPolygons(&nPolygonsNum);

		if (nPolygonsNum <= 0)
			return MIN_TIME_COST;

		for (int i = nPolygonsNum - 1; i >= 0; i--)
		{
			DrawFillPolygon(pPolygons[i], x, y, zoom, grid);
		}

		DeletePolygons(pPolygons, nPolygonsNum);
		
		double cost = (double)(clock() - t);
		if(cost <= 0)
			cost = MIN_TIME_COST;

		return cost / CLOCKS_PER_SEC;
	}

};

_HD3D_END
