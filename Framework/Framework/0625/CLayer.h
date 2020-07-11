#pragma once
#include<vector>
using namespace std;

class CLayer
{
public:
	CLayer(const char* _szFileName);
	~CLayer();

public:
	void Render(const HDC& _hdc);
	vector<vector<pair<INT, INT>>> m_vecDatas;
};

