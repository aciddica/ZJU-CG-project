#include <iostream>
#include <sstream>
#include <string>
#include "ReadObj.h"
using namespace std;
int numOfFaces = 0;
/*for test*/
int cnt_faces = 0;
/*end*/
unsigned int texture[100];
unsigned char* LoadFileContent(const char* path, int& filesize)   //�ú����������ȹȽ̳�
//��ȡ�ļ��������ļ����ݣ����ļ���С��ֵ��filesize       
{
	unsigned char* fileContent = nullptr;
	filesize = 0;
	FILE* pFile = NULL;
	errno_t err;
	err = fopen_s(&pFile, path, "rb");
	if (err) cout << "obj/mtl�ļ���ʧ�ܣ�";
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);      //���ļ�ָ���ƶ����ļ�ĩβ
		int nLen = ftell(pFile);        //�����ļ�ͷ���ľ���   //����ָ�ļ���С
		if (nLen > 0)
		{
			rewind(pFile);          //�ƶ����ļ�ͷ��
			fileContent = new unsigned char[nLen + 1];
			//Ϊ�ļ�ָ�뿪�ٿռ�
			fread(fileContent, sizeof(unsigned char), nLen, pFile);
			//��pFile�����ݶ���fileContent
			fileContent[nLen] = '\0';         //�ļ�ĩβ����\0
			filesize = nLen;                  //Ϊ�ļ���С��ֵ
		}
		fclose(pFile);
	}
	return fileContent;
}
unsigned char* LoadBitmapFile(char* filename, BITMAPINFOHEADER* bitmapInfoHeader)
{
	FILE* filePtr;	// �ļ�ָ��
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap�ļ�ͷ
	unsigned char* bitmapImage;		// bitmapͼ������
	int	imageIdx = 0;		// ͼ��λ������
	unsigned char	tempRGB;	// ��������

								// �ԡ�������+����ģʽ���ļ�filename 
	fopen_s(&filePtr, filename, "rb");
	if (filePtr == NULL) return NULL;
	// ����bitmap�ļ�ͼ
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// ��֤�Ƿ�Ϊbitmap�ļ�
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// ����bitmap��Ϣͷ
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// ���ļ�ָ������bitmap����
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// Ϊװ��ͼ�����ݴ����㹻���ڴ�
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// ��֤�ڴ��Ƿ񴴽��ɹ�
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// ����bitmapͼ������
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// ȷ�϶���ɹ�
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//����bitmap�б���ĸ�ʽ��BGR�����潻��R��B��ֵ���õ�RGB��ʽ
	for (imageIdx = 0;
		imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// �ر�bitmapͼ���ļ�
	fclose(filePtr);
	return bitmapImage;
}
void texload(int i, char* filename)
{

	BITMAPINFOHEADER bitmapInfoHeader;                                 // bitmap��Ϣͷ
	unsigned char* bitmapData;                                       // ��������

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	// ָ����ǰ����ķŴ�/��С���˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap���(ͨ��Ϊ0����ʾ���ϲ�) 
		GL_RGB,	//����ϣ���������к졢�̡�������
		bitmapInfoHeader.biWidth, //��������������n�����б߿�+2 
		bitmapInfoHeader.biHeight, //����߶ȣ�������n�����б߿�+2 
		0, //�߿�(0=�ޱ߿�, 1=�б߿�) 
		GL_RGB,	//bitmap���ݵĸ�ʽ
		GL_UNSIGNED_BYTE, //ÿ����ɫ���ݵ�����
		bitmapData);	//bitmap����ָ��  
}
objFile::objFile(const char* objFileName)
{
	int nFileSize = 0;
	unsigned char* objFileContent = LoadFileContent(objFileName, nFileSize);
	if (objFileContent == nullptr) {
		return;
	}
	stringstream ssobjFileContent((char*)objFileContent);//��������ļ����ַ�����
	string head = "";
	int cnt_g = 0;
	objModel tempobjModel;//����ģ�Ͷ���ÿ��g�����ļ�ĩβ��ʱ��������objs
	char szoneLine[256];//һ�е��ַ���
	while (!ssobjFileContent.eof())
	{
		memset(szoneLine, 0, 256);//ÿ��ѭ����ʼ������
		ssobjFileContent.getline(szoneLine, 256, '\n');//������ȡһ��
		if (strlen(szoneLine) > 0)
		{
			stringstream ssOneLine(szoneLine);//һ��ת��Ϊ��
			ssOneLine >> head;//��ȡͷ!!!��������޷���ȡͷ��ʲôҲ���������Ǳ���֮ǰ��״̬��
			//cout << "head==" << head << endl;
			//cout << "szoneLine==  ";
			//cout << szoneLine << endl;
			if (head == "mtllib") {//ָ����mtl�ļ�
				string mtlFileName;
				ssOneLine >> mtlFileName;
				char* p = &mtlFileName[0];

				/*handle_mtlFile(p);*///����objָ����mtl�ļ�
				int mtlFileSize = 0;
				unsigned char* mtlFileContent = LoadFileContent(p, mtlFileSize);//��.mtl�ļ�������
				//cout << mtlFileContent << endl;
				if (mtlFileContent == nullptr)
				{
					head = "";
					continue;
				}
				stringstream ssmtl((char*)mtlFileContent);
				char szoneLine_mtl[256];
				string key;//���ʵ�����
				while (!ssmtl.eof())
				{
					memset(szoneLine_mtl, 0, 256);        //  ÿ��ѭ����ʼ������szoneLine
					ssmtl.getline(szoneLine_mtl, 256);      //����ȡһ��
					if (strlen(szoneLine_mtl) > 0)       //���в�Ϊ��
					{
						stringstream ssOneLine_mtl(szoneLine_mtl);
						string head_mtl;
						ssOneLine_mtl >> head_mtl;
						if (head_mtl == "newmtl") {
							ssOneLine_mtl >> key;
						}
						else if (head_mtl == "Ka") {
							ssOneLine_mtl >> mtlMap[key].Data[0][0] >> mtlMap[key].Data[0][1] >> mtlMap[key].Data[0][2];
						}
						else if (head_mtl == "Kd") {
							ssOneLine_mtl >> mtlMap[key].Data[1][0] >> mtlMap[key].Data[1][1] >> mtlMap[key].Data[1][2];
						}
						else if (head_mtl == "Ks") {
							ssOneLine_mtl >> mtlMap[key].Data[2][0] >> mtlMap[key].Data[2][1] >> mtlMap[key].Data[2][2];
						}
						else if (head_mtl == "Ns") {
							ssOneLine_mtl >> mtlMap[key].Data[3][0];
						}
						else if (head_mtl == "map_Kd") {
							ssOneLine_mtl >> mtlMap[key].Kd_pic;
						}
						else if (head_mtl == "#") {
							continue;
						}
					}
				}
				/*������*/
				//1.�������
				int cnt_texture = 0;
				for (auto p = mtlMap.begin(); p != mtlMap.end(); p++)
				{
					if (p->second.Kd_pic != "") {
						cnt_texture++;
					}
				}
				glGenTextures(cnt_texture, texture);//��������һ�µ�texture
				int cnt_loadtexture = 0;
				for (auto p = mtlMap.begin(); p != mtlMap.end(); p++) {
					if (p->second.Kd_pic != "") {
						/*������ͼ*/
						p->second.Kd_pic_flag = cnt_loadtexture;
						char* q = &(p->second.Kd_pic[0]);
						texload(cnt_loadtexture++, q);
					}
				}
				delete mtlFileContent;

			}
			else if (head == "v") {//������Ϣ
				Float3 tempLocation;
				ssOneLine >> tempLocation.Data[0] >> tempLocation.Data[1] >> tempLocation.Data[2];
				mLocation.push_back(tempLocation);
			}
			else if (head == "vn") {//������Ϣ
				Float3 tempNormal;
				ssOneLine >> tempNormal.Data[0] >> tempNormal.Data[1] >> tempNormal.Data[2];
				mNormal.push_back(tempNormal);
			}
			else if (head == "vt") {//����������Ϣ
				Float2 tempTexcoord;
				ssOneLine >> tempTexcoord.Data[0] >> tempTexcoord.Data[1];
				mTexcoord.push_back(tempTexcoord);
			}
			else if (head == "usemtl") {//usemtl֮�����µ��������
				string tempnameString;
				ssOneLine >> tempnameString;
				tempobjModel.set_nameString(&tempnameString);//���ö�Ӧ��mtl�ò��ʵ�����
			}
			else if (head == "g") {
				if (cnt_g) {
					objs.push_back(tempobjModel);
					tempobjModel.clear_faceVector();//���tempobjModel��vector
				}
				cnt_g++;
			}
			else if (head == "f") {
				cnt_faces++;
				//cout << "length="<<strlen(szoneLine) << endl;
				string vertexStr;   //������������
				Face tempFace;      //��Ƭ�ṹ

				/*�ж���������Ƭ�����ı�����Ƭ*/
				int cnt_blank = 0;
				for (int k = 0; szoneLine[k] != '\0'; k++) {
					if (szoneLine[k] == ' ') cnt_blank++;
				}
				//cout << "cnt_blank==" << cnt_blank << "  ";
				//cout << "szoneLine=="<< szoneLine <<endl;
				numOfFaces = cnt_blank;
				if (numOfFaces == 3) {
					for (int i = 0; i < 3; ++i)         //ÿ����������
					{
						ssOneLine >> vertexStr;           //�����ж�ȡ���������Ϣ
						size_t pos = vertexStr.find_first_of('/');       //�ҵ���һ��/��λ��      //���ҵ����λ����Ϣ
						string locIndexStr = vertexStr.substr(0, pos);       //��ֵ���λ����Ϣ
						size_t pos2 = vertexStr.find_first_of('/', pos + 1);   //�ҵ��ڶ���/   ���ҵ��������������Ϣ
						string texIndexSrt = vertexStr.substr(pos + 1, pos2 - 1 - pos);       //��ֵ�������������Ϣ!!!!��ע���������������м�!!!��
						string norIndexSrt = vertexStr.substr(pos2 + 1, vertexStr.length() - 1 - pos2);   //��ֵ��ķ�����Ϣ
						tempFace.vertex[i][2] = atoi(norIndexSrt.c_str());         //��������������Ϣ��ֵ
						tempFace.vertex[i][1] = atoi(texIndexSrt.c_str());         //������Ϣ��ֵ
						tempFace.vertex[i][0] = atoi(locIndexStr.c_str());          //��������Ϣ�� srtingת��Ϊ int     //λ��������Ϣ��ֵ


					}
					tempobjModel.push_face(&tempFace);
				}
				else if (numOfFaces == 4) {
					for (int i = 0; i < 4; ++i)         //ÿ����������
					{
						ssOneLine >> vertexStr;           //�����ж�ȡ���������Ϣ
						size_t pos = vertexStr.find_first_of('/');       //�ҵ���һ��/��λ��      //���ҵ����λ����Ϣ
						string locIndexStr = vertexStr.substr(0, pos);       //��ֵ���λ����Ϣ
						size_t pos2 = vertexStr.find_first_of('/', pos + 1);   //�ҵ��ڶ���/   ���ҵ��������������Ϣ
						string texIndexSrt = vertexStr.substr(pos + 1, pos2 - 1 - pos);       //��ֵ�������������Ϣ
						string norIndexSrt = vertexStr.substr(pos2 + 1, vertexStr.length() - 1 - pos2);   //��ֵ��ķ�����Ϣ
						tempFace.vertex[i][2] = atoi(norIndexSrt.c_str());         //������Ϣ��ֵ
						tempFace.vertex[i][1] = atoi(texIndexSrt.c_str());         //��������������Ϣ��ֵ
						tempFace.vertex[i][0] = atoi(locIndexStr.c_str());          //��������Ϣ�� srtingת��Ϊ int     //λ��������Ϣ��ֵ
					}
					tempobjModel.push_face(&tempFace);
				}

			}
			else if (head == "#") {
				head = "";
				continue;
			}
			head = "";
		}
	}
	objs.push_back(tempobjModel);
	delete objFileContent;
	cout << "mLocationSize= " << mLocation.size() << endl;
	cout << mLocation[0].Data[0] << " " << mLocation[0].Data[1] << " " << mLocation[0].Data[2] << endl;
	cout << "mNormalSize= " << mNormal.size() << endl;
	cout << mNormal[0].Data[0] << " " << mNormal[0].Data[1] << " " << mNormal[0].Data[2] << endl;
	cout << "mTexcoordsize= " << mTexcoord.size() << endl;
	cout << mTexcoord[0].Data[0] << " " << mTexcoord[0].Data[1] << " " << mTexcoord[0].Data[2] << endl;
	cout << "cnt_faces=" << cnt_faces << endl;
	cout << "face[0]" << objs[0].get_mFace()->begin()->vertex[0][0] << " " << objs[0].get_mFace()->begin()->vertex[0][1] << " " << objs[0].get_mFace()->begin()->vertex[0][2] << endl;
	cout << "face[0]" << objs[0].get_mFace()->begin()->vertex[1][0] << " " << objs[0].get_mFace()->begin()->vertex[1][1] << " " << objs[0].get_mFace()->begin()->vertex[1][2] << endl;
	cout << "face[0]" << objs[0].get_mFace()->begin()->vertex[2][0] << " " << objs[0].get_mFace()->begin()->vertex[2][1] << " " << objs[0].get_mFace()->begin()->vertex[2][2] << endl;
}
void objFile::objFileDraw()
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	for (int i = 0; i < objs.size(); i++)
	{
		/*������ӳ��*/
		if (mtlMap[(*(objs[i].get_nameString()))].Kd_pic != "") {
			GLuint texture_enum = mtlMap[(*(objs[i].get_nameString()))].Kd_pic_flag;
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture[texture_enum]);
		}
		/*���ò���*/
		glMaterialfv(GL_FRONT, GL_AMBIENT, mtlMap[(*(objs[i].get_nameString()))].Data[0]);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mtlMap[(*(objs[i].get_nameString()))].Data[1]);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mtlMap[(*(objs[i].get_nameString()))].Data[2]);
		glMaterialfv(GL_FRONT, GL_SHININESS, mtlMap[(*(objs[i].get_nameString()))].Data[3]);
		//cout << mtlMap[(*(objs[i].get_nameString()))].Data[0][0] << " " << mtlMap[(*(objs[i].get_nameString()))].Data[0][1] << " " << mtlMap[(*(objs[i].get_nameString()))].Data[0][2] << endl;
		//cout << mtlMap[(*(objs[i].get_nameString()))].Data[1][0] << " " << mtlMap[(*(objs[i].get_nameString()))].Data[1][1] << " " << mtlMap[(*(objs[i].get_nameString()))].Data[1][2] << endl;
		//cout << mtlMap[(*(objs[i].get_nameString()))].Data[2][0] << " " << mtlMap[(*(objs[i].get_nameString()))].Data[2][1] << " " << mtlMap[(*(objs[i].get_nameString()))].Data[2][2] << endl;
		//cout << mtlMap[(*(objs[i].get_nameString()))].Data[3][0] << " " << mtlMap[(*(objs[i].get_nameString()))].Data[3][1] << " " << mtlMap[(*(objs[i].get_nameString()))].Data[3][2] << endl;

		/*��㻭ͼ*/
		if (numOfFaces == 3) {
			glBegin(GL_TRIANGLES);
			for (auto faceIndex = objs[i].get_mFace()->begin(); faceIndex != objs[i].get_mFace()->end(); ++faceIndex)
			{
				//��һ����ķ��ߣ�����λ����Ϣ
				glNormal3fv(mNormal[faceIndex->vertex[0][2] - 1].Data);
				glTexCoord2fv(mTexcoord[faceIndex->vertex[0][1] - 1].Data);
				glVertex3fv(mLocation[faceIndex->vertex[0][0] - 1].Data);
				//�ڶ�����ķ��ߣ�����λ����Ϣ
				glNormal3fv(mNormal[faceIndex->vertex[1][2] - 1].Data);
				glTexCoord2fv(mTexcoord[faceIndex->vertex[1][1] - 1].Data);
				glVertex3fv(mLocation[faceIndex->vertex[1][0] - 1].Data);
				//��������ķ��ߣ�����λ����Ϣ
				glNormal3fv(mNormal[faceIndex->vertex[2][2] - 1].Data);
				glTexCoord2fv(mTexcoord[faceIndex->vertex[2][1] - 1].Data);
				glVertex3fv(mLocation[faceIndex->vertex[2][0] - 1].Data);
			}
			glEnd();
		}
		else if (numOfFaces == 4) {
			glBegin(GL_QUADS);
			for (auto faceIndex = objs[i].get_mFace()->begin(); faceIndex != objs[i].get_mFace()->end(); ++faceIndex)
			{
				//��һ����ķ��ߣ�����λ����Ϣ
				glNormal3fv(mNormal[faceIndex->vertex[0][2] - 1].Data);
				glTexCoord2fv(mTexcoord[faceIndex->vertex[0][1] - 1].Data);
				glVertex3fv(mLocation[faceIndex->vertex[0][0] - 1].Data);
				//�ڶ�����ķ��ߣ�����λ����Ϣ
				glNormal3fv(mNormal[faceIndex->vertex[1][2] - 1].Data);
				glTexCoord2fv(mTexcoord[faceIndex->vertex[1][1] - 1].Data);
				glVertex3fv(mLocation[faceIndex->vertex[1][0] - 1].Data);
				//��������ķ��ߣ�����λ����Ϣ
				glNormal3fv(mNormal[faceIndex->vertex[2][2] - 1].Data);
				glTexCoord2fv(mTexcoord[faceIndex->vertex[2][1] - 1].Data);
				glVertex3fv(mLocation[faceIndex->vertex[2][0] - 1].Data);
				//���ĸ���ķ��ߣ�����λ����Ϣ
				glNormal3fv(mNormal[faceIndex->vertex[3][2] - 1].Data);
				glTexCoord2fv(mTexcoord[faceIndex->vertex[3][1] - 1].Data);
				glVertex3fv(mLocation[faceIndex->vertex[3][0] - 1].Data);
			}
			glEnd();
		}
		/*�ر�����ӳ��*/
		glDisable(GL_TEXTURE_2D);
	}
}
