#ifndef _READOBJ_H_
#define _READOBJ_H_
#include<windows.h>
#include <GLFW\glfw3.h>
#include<vector>
#include<map>
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define M_PI 3.14159
#define BITMAP_ID 0x4D42
using namespace std;

struct Float3       //���λ����Ϣ�ͷ�����Ϣ����������
{
	float Data[3];     //x,y,z
};
struct Float2      //�������������������
{
	float Data[2];   //u,v
};
struct Face          //����Ϣ
{
	int vertex[4][3];       //�����㹹��һ����  ÿ����������������Ϣ
};
struct Material
{
	float Data[4][3];
	string Kd_pic = "";
	GLuint Kd_pic_flag = -1;
};

/*
	.obj�ļ��ڵĵ������壺������Ƭ��Ϣ��+������ͼ�����ϡ�
*/
class objModel
{
public:
	void push_face(Face* f)
	{
		mFace.push_back(*f);
	}
	void clear_faceVector() {
		int len = mFace.size();
		for (int i = 0; i < len; i++) {
			mFace.pop_back();
		}
	}
	void set_nameString(string* s)
	{
		Material_nameString = *s;
	}
	vector<Face>* get_mFace() {
		return &mFace;
	}
	string* get_nameString() {
		return &Material_nameString;
	}
private:
	vector<Face> mFace;              //������Ƭ�ļ���
	string Material_nameString;      //���������ַ���
};
/*
	����.obj�ļ����������ж�����Ϣ+������Ϣ+����������Ϣ��+ ���塿�ļ��ϡ�
*/
class objFile
{
public:
	objFile(const char* objFileName);//����objFile����
	void objFileDraw();              //����obj�ļ�����������
private:
	vector<Float3> mLocation;        //���е����Ϣ
	vector<Float3> mNormal;          //���з�����Ϣ
	vector<Float2> mTexcoord;        //��������������Ϣ
	vector<objModel> objs;           //���е�����
	map<string, Material> mtlMap;   //��.obj��ص�.mtl�ļ������в�����Ϣ�������������������Ϣ��ֻҪ��չMaterial���ɡ�
};

/*
	�ļ���ȡ
*/
unsigned char* LoadFileContent(const char* path, int& filesize);
unsigned char* LoadBitmapFile(char* filename, BITMAPINFOHEADER* bitmapInfoHeader);
void texload(int i, char* filename);

#endif