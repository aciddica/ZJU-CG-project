/*Add by KQ*/
#ifndef _MYGLM_H_
#define _MYGLM_H_
/*end*/
#include <math.h>
#include <vector>
#include<iostream>
#include <GLFW\glfw3.h>
#include<stdio.h>
#define MAPSIZE  12
#define MAPBOUND 3000.0
#define GRIDLEN (MAPBOUND / MAPSIZE)
#define PIPE_GRID_TIME 2 //pipeline��һ��С���ǵ�ͼС��ļ�����
#define PIPESIZE (MAPSIZE/PIPE_GRID_TIME)
extern double CameraPosition[];
extern double CameraDirection[];
extern double TargetDistance;
/*add by KQ*/
extern int view_list[];
extern int top;
/*end*/
void Stage();
void cross(double v1[], double v2[], double res[]);
void normalize(double v[]);
//double* CameraMove(int mode);
class Point //the class point
{
public:
	double x, y, z;
	Point();
	Point(double ix, double iy, double iz);
};
class MapUnit //the class of grid in the map
{
public:
	double GirdLen = MAPBOUND / MAPSIZE;
	Point leftbottom;
	Point rightup;
	int status;
	MapUnit();
	MapUnit(int s, Point lu, Point rb);
	void drawGrid();
	void drawModel();
};
class Map
{
public:
	double GirdLen = MAPBOUND / MAPSIZE;
	MapUnit map[MAPSIZE][MAPSIZE];
	Map();
	void drawMap();
	void Select(double x, double y, int status);	//mapping the point to the map surface(z = 0)
};



class PipelineUnit//��ˮ�ߵ�Ԫ
{
public:
	Point leftbottom;
	Point rightup;
	int isMachine = 0;//�Ƿ����м�Ĵ������
	double Len = PIPE_GRID_TIME * GRIDLEN;//ÿ����ˮ�������Ԫ��ĳ���

	int type;
	int modetype;
	//��ˮ�ߵĲ������� ����ˮ�������з�����ݲ�ͬ�����ֻ��Ʋ�ͬ����ˮ����� 
	//0��û����ˮ�� 1����ˮ����� ����up 2����ˮ����� ����left 3����ˮ����� ����down 4����ˮ����� ����right
	//5����ˮ���յ� ����up 6����ˮ���յ� ����left 7����ˮ���յ� ����down 8����ˮ���յ� ����right 
	//9����ͨ��ˮ�߹��up->down����  10: ��ͨ��ˮ�߹��left->right���� 11����ͨ��ˮ�߹��left->up����
	//12����ͨ��ˮ�߹��left->down���� 13: ��ͨ��ˮ�߹��right->up���� 14����ͨ��ˮ��right->down����
	//15����ʼ�����  
	//16 ĳЩ��ˮ���ϵĴ���װ�á�
	//17����ͨ��ˮ�߹��down->up����  18: ��ͨ��ˮ�߹��right->left���� 19����ͨ��ˮ�߹��up->left����
	//20����ͨ��ˮ�߹��down->left���� 21: ��ͨ��ˮ�߹��up->right���� 22����ͨ��ˮ��down->right����
	//23��������������ˮ���ϵĴ���װ��

	PipelineUnit();
	PipelineUnit(int s, Point lu, Point rb);
	void ChangeType(int s);
	void drawModel();//���ݲ�ͬ��type���Ʋ�ͬ����ˮ����ɲ���
};

class Product//�������Ĺ���
{
public:
	//int isLive = 0;//��������Ƿ����
	int isPipeLine = 0;//����ط��Ƿ�����ˮ�ߵ�Ԫ
	int isMachine = 0;//�����ˮ�ߵ�Ԫ�Ƿ��Ǽӹ���
	double moveRatio = 0;//������ĳ����ˮ�ߵ�Ԫ���˶���ɵİٷֱȣ�����50%������һ��
	double movespeed = 0.1;//ÿ���ƶ��ƶ���ratio�����൱���ٶ�
	double pipelineZline = 0.3;//��ˮ�ߵĺ�ȣ���ֹ��������ˮ���ص���
	double large = 0.2;//���Ļ���Ĵ�С
	int hasPaint = 0;//�Ƿ񱻻�����
	int direction;//��������һ��λ���ڵ�ǰλ�õ��ĸ�����0��û����һ��λ��
	//1��up 2��left 3��down 4��right
	Point moveBegin, moveEnd;//�������ˮ�ߵ�Ԫ�ϵ��˶���ʼ��ͽ�����

	Product();
	Product(PipelineUnit beginUnit);//��ĳ����ˮ�߽ڵ��ʼ��������

	//void MoveProduct();//�������ϸı�λ��
	void drawProduct();//��������

};

class PipelineMap//��ˮ�ߵ�ͼ
{
public:
	double Len = PIPE_GRID_TIME * GRIDLEN;
	PipelineUnit map[PIPESIZE][PIPESIZE];

	int haveBegin = 0;//�Ƿ��������
	std::vector<int> BeginPoint;//�������꣬����֮���������յ��˶�
	int isEnd = 0;//��ˮ���Ƿ�����

	std::vector<int> nowEndPoint;
	//��δ���꣬���ǻ�����ˮ����ʱĩ�˵����ꣻ���ѻ��꣬������ˮ�������յ�����꣬����֮���������յ��˶���

	int paintMode = 0;//����ģʽ����Ϊ0��������ٻ滭����Ϊ1���������Դ���ʱ����ˮ��ĩ����������ĵ�������ˮ��
	//��Ϊ2������Խ���ˮ�����Ѿ����ڵ���ͨ��ˮ�߹����Ϊĳ�ּӹ�������
	//��Ϊ3������ƽ������޷��ٸı���ˮ�ߡ�
	int showmode = 0; //0��ֻ������ˮ�ߣ�1�������ˮ�����˶�������
	PipelineMap();
	void drawMap();
	void Select(double x, double y);
};

class ProductMap//��Ʒ��ͼ
{
public:
	double Len = PIPE_GRID_TIME * GRIDLEN;
	Product map[PIPESIZE][PIPESIZE];

	std::vector<int> BeginPoint;//�������꣬����֮���������յ��˶�
	std::vector<int> nowEndPoint;//��Ʒ�յ�����
	std::vector<int> nowPaint;//���ڻ��Ƶ���Ʒ������
	int hasPaint = 0;
	int showmode = 0; //0��ֻ������ˮ�ߣ�1�������ˮ�����˶�������
	//PipeMap();
	ProductMap();
	ProductMap(PipelineMap pipemap);
	void drawMap();
};
#endif